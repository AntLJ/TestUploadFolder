/* Copyright (c) Oracle Corporation 2002.  All Rights Reserved */

/*
    Please see the online help for documentation of these classes.
*/

/*
    Oracle Objects for OLE     C++ Classes
    
    This file is the header for the OBound and OBinder classes
                           
    CREATED    ********   11/22/94
    RWOOLARD	MODIFIED	03/20/95
    			bug#	262553	Added more methods to access the bound dynaset
    PCHARI	MODIFIED	05/06/99
    			bug#	852923	Added boolean to obinder to check if database was opened

*/

#ifndef OBOUND_ORACLE
#define OBOUND_ORACLE

#ifndef ORACL_ORACLE
#include "oracl.h"
#endif

// declarations for forward references
class OBinderAdvise;
class OEXPORT OBinder;

// ----- OBound -----------------------------------------------

class OEXPORT OHUGESP OBound
{
    friend class OBinder;
    friend class OBinderAdvise;

public:
    OBound(void);
    ~OBound(void);
    
    // to attach bound object to a binder
    oresult BindToBinder(OBinder *binder, const char *fieldname);
    // to detach
    oresult Unbind(void); 

    oresult Changed(oboolean changed = TRUE);
    
    const char *GetName(void) const;  // get name of field we're attached to
    ODatabase GetDatabase(void) const;
    ODynaset GetDynaset(void) const;
    
    oboolean IsChanged(void) const;
    
    // declarations of methods so that compiler won't implement these (which would be wrong)
    // at present we don't implement these either...
    OBound(const OBound &other);  // copy constructor
    OBound &operator=(const OBound &other);  // overloaded assignment
    
protected:
    // helper routines for subclasses
    oresult GetValue(OValue *val);       // get current value from database
    oresult SetValue(const OValue &val); // set value in database
    oresult RefreshBound(void);              // get value and call Refresh method
    
    // routines subclasses need to implement
    virtual oresult Refresh(const OValue &val) = 0;  // database -> bound
    virtual oresult SaveChange(void) = 0;  // bound -> database

private:
    oboolean m_changed;     // flag indicating we've had a change
    OBinder *m_binder;      // pointer to containing binder
    char    *m_fieldname;   // name of field we're bound to
    int      m_fieldindex;  // index of field we're bound to
    
    oresult SaveChangeBound(void);    
    oresult GetFieldIndex(void);
    oresult UnbindNotify(void); 
    
    oresult ClearChange(void);

protected:    
    /*
        Triggers.  These routines are called by OBinder.  At some in OBinder
        these triggers will be fired for all of the objects in a binder.  The
        triggers fire after the corresponding binder triggers (with the exception
        of the Startup and Shutdown triggers). The firing order of the bound objects
        is the order that they were bound to the binder.  If any trigger in a binder
        does not return OSUCCESS, processing will be halted.
        
        The routines are all listed below as
        method
        Fired:  when the trigger fires
        Default: the action taken by the base OBound class method
    */
    virtual oresult Startup(void);
    // Fired: immediately after bound object is bound to binder.  If the binder has not
    //        been opened then GetValue can't be called  
    // Default: no action
    virtual oresult Shutdown(void);
    // Fired: when bound object is unbound from binder, either by an explicit call to
    //        OBinder::UnbindObject or when OBinder::Close is called
    // Default: no action
    virtual oresult PreQuery(void);
    // Fired: before the SQL statement is executed
    // Default: no action
    virtual oresult PostQuery(void);
    // Fired: after SQL statement is executed and new bindings are established
    // Default: refreshes bound object 
    virtual oresult PreDelete(void);
    // Fired: before record is deleted
    // Default: no action
    virtual oresult PostDelete(void);
    // Fired: after record is deleted (but not committed to database)
    // Default: no action
    virtual oresult PreAdd(void);
    // Fired: before new record is added
    // Default: no action
    virtual oresult PostAdd(void);
    // Fired: after a new record is added
    // Default: refreshes bound object - value will generally be NULL
    virtual oresult PreUpdate(void);
    // Fired: just before database is updated
    // Default: no action
    virtual oresult PostUpdate(void);
    // Fired: immediately after database is updated
    // Default: no action
    virtual oresult PreRollback(void);
    // Fired: immediately before a rollback is attempted
    // Default: no action
    virtual oresult PostRollback(void);
    // Fired: immediately after a rollback
    // Default: refresh bound object 
    virtual oresult PreMove(void);
    // Fired: before OBinder moves to a new record
    // Default: no action
    virtual oresult PostMove(void);
    // Fired: after OBinder moves to a new record
    // Default: refreshes bound object 

};

// ----- OBinder -----------------------------------------------

class OEXPORT OBinder
{
    friend class OBinderAdvise;
    friend OBound::BindToBinder(OBinder *binder, const char *fieldname);  // so can call AddToOBinder 

public:
    OBinder(void);
    ~OBinder(void);
    
    // start (or restart) a query on the binder
    oresult Open(const char *dbname, const char *username, const char *pwd, const char *sqls, long dynoptions = ODYNASET_DEFAULT);  // data control form
	 oresult Open(const ODatabase &odb, const char *sqls, long dynoptions = ODYNASET_DEFAULT);  // using an existing database
//	Added for V2DEV custom dynaset
	 oresult Open(const ODatabase &odb, const char *sql_statement,
					unsigned int slicesize, unsigned int perblock,
					unsigned int blocks, unsigned int fetchlimit,
					unsigned int fetchsize, long options = ODYNASET_DEFAULT);

    oresult Close(oboolean doShutdown = TRUE);
    
    // for resetting the query of the block
    oresult SetSQL(const char *sqls);  // just set the SQL for the next query 
//BUG #262553
    const char *GetSQL(void) const;  // gets sql statement
    oresult RefreshQuery(void);  // refresh query
    
    oresult Refresh(void);  // get values to all bound objects
    
    // navigational methods
    //   (can also call methods on the dynaset returned by GetDynaset)
    oresult MoveFirst(void);
    oresult MovePrev(void);
    oresult MoveNext(void);
    oresult MoveLast(void);
//	V2DEV - extra move methods
	 oresult MovePrevN(long rows);   // go to previous (n) record
	 oresult MoveNextN(long rows);   // go to next (n) record
	 oresult MoveRel(long rows);		// go to relative record (n)
	 oresult MoveTo(long rownum);    // go to record (n)

	 oresult MoveToMark(const ODynasetMark &mark);

    // ask for the binder's dynaset
    const ODynaset GetDynaset(void) const;
    const ODatabase GetDatabase(void) const;
    
    // unbind an object
    oresult UnbindObj(OBound *object, oboolean nofail = FALSE);
    
    // get or set the value of a field (primarily used by OBound)
    oresult GetFieldValue(int index, OValue *val) const;
    oresult SetFieldValue(int index, const OValue &val);
    
    // routine called by a child bound object to get it's field
    oresult GetFieldIndex(int *ofldi, const char *fieldname);
    
    oresult DeleteRecord(void);
    oresult AddNewRecord(void);
    oresult DuplicateRecord(void);

 // V2DEV -	Find methods
 	 oresult FindFirst(const char *sql);
	 oresult FindNext(const char *sql = 0);
	 oresult FindPrevious(const char *sql = 0);
	 oresult FindLast(const char *sql);
	 oboolean NoMatch(void);							// TRUE if find failed

    oresult	 Changed(oboolean cflag = TRUE);
    oboolean IsChanged(void) const;
//BUG #262553
	oboolean IsOpen(void);
    oboolean IsFirst(void);
    oboolean IsLast(void);
    	
    oboolean GetChangedError(long *serr, long *cerr) const; 
    
	// save changes, if any
    oresult Update(void);
    // throw away changes, if any
	 oresult DiscardChanges(void);
    
    // Error handling methods    
//BUG #262553
    long  ErrorNumber(void) const; // return error "number"	
    const char *LookupErrorText(long errnum) const;
										  // get error text for given error number
    const char *GetErrorText(void) const;  // get description of last error
    // set error information
    void  ErrorReset(void) const;  // reset error state to "no error"

    oboolean CanMark(void) const;
    ODynasetMark GetMark(void) const;  // bookmark at current position
    ODynasetMark GetLastModifiedMark(void) const ;	// get bookmark at last modified record

    // record access
//BUG #262553
	 int GetFieldCount(void) const;  // returns # of fields in a record
    long GetRecordCount(void) const;
    // (dangerous!) gets total number of records in dynaset -- downloads the entire dynaset to the client   
    int GetFieldIndex(const char *fieldname) const;
    												// gets the index of a field by name
        
    // declarations of methods so that compiler won't implement these (which would be wrong)
    // at present we don't implement these either...
    OBinder(const OBinder &other);  // copy constructor
    OBinder &operator=(const OBinder &other);  // overloaded assignment

protected:
    /*
        Error handling routine.  This routine is called if there is an error during processing
        of Changed.
    */             
    virtual void OnChangedError(void);
    
    /*
        Trigger routines.  These routines are all fired on some action
        They are all documented to show:
        
        Fired: when the trigger fires
        Default: the action of the OBinder base class
        
        The triggers all return an oresult.  If the trigger does not return
        OSUCCESS the current action is cancelled.
        
        After a binder trigger routine is called, the same trigger is called
        on each of the bound objects (routines in OBound), with the exception of
        Startup and Shutdown.  Currently the order
        the triggers are called on the bound objects is not guaranteed.  The
        object triggers will not be called if the binder trigger fails.  If any
        of the object triggers fail, no more processing will be done
    */
    
    virtual oresult Startup(void);
    // Fired: at the time the first bound object is added to the binder
    //        guaranteed to only be called once
    // Default: no action
    virtual oresult Shutdown(void);
    // Fired: when OBinder::Close is called
    // Default: if changes have been made, save them (calls OBound::SaveChange on every object) and
    //          update the record (which as a side effect calls the update triggers)
    virtual oresult PreQuery(void);
    // Fired: before the SQL statement is executed
    // Default: no action
    virtual oresult PostQuery(void);
    // Fired: after SQL statement is executed and new bindings are established
    // Default: no action
    virtual oresult PreDelete(void);
    // Fired: before record is deleted
    // Default: no action
    virtual oresult PostDelete(void);
    // Fired: after record is deleted (but not committed to database)
    // Default: no action
    virtual oresult PreAdd(void);
    // Fired: before new record is added
    // Default: if changes have been made, save them (calls OBound::SaveChange on every object) and
    //          update the record (which as a side effect calls the update triggers)
    virtual oresult PostAdd(void);
    // Fired: after a new record is added
    // Default: no action
    virtual oresult PreUpdate(void);
    // Fired: just before database is updated
    // Default: no action
    virtual oresult PostUpdate(void);
    // Fired: immediately after database is updated
    // Default: no action 
    virtual oresult PreRollback(void);
    // Fired: immediately before a rollback is attempted
    // Default: no action
    virtual oresult PostRollback(void);
    // Fired: immediately after a rollback
    // Default: no action
    virtual oresult PreMove(void);
    // Fired: before OBinder moves to a new record
    // Default: if changes have been made, save them (calls OBound::SaveChange on every object) and
    //          update the record (which as a side effect calls the update triggers)
    virtual oresult PostMove(void);
    // Fired: after OBinder moves to a new record
    // Default: no action
    

private:
    ODynaset      m_dynaset;
    ODatabase     m_database;
    OBound      **m_boundlist;
    void           *m_advise;     // pointer to our special advisory
    
    oboolean        m_changed;  // TRUE if binder has been changed since an update
	// Bug852923
	oboolean        m_bOpenedDatabase;  // TRUE if database open is called by obinder
    int             m_nbound;
    
    // errors saved from change
    long         m_changeerr;  // class lib error
    long         m_schangeerr; // server error
    
    // Bind an object to the binder (called by OBound)
    oresult AddObjectToOBinder(OBound *object);

    // operations on all the bound objects
    oresult DoAllObjects(oresult (OBound::*ff)(void));  // operate on all via dynaset ref

    // routines used to implement default behavior
    oresult SaveRecordChanges(void);
};

#endif // OBOUND_ORACLE

