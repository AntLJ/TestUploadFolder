DOXYGEN_SOURCES = make.bat makefile doxyfile *.dox ../*.rc

Release : $(INTDIR)/$(RELEASEFILENAME).chm
Develop : $(INTDIR)/$(DEVFILENAME).chm

$(INTDIR)/$(RELEASEFILENAME).chm : $(DOXYGEN_SOURCES)
	@set TARGETFILENAME=$(@F)
	@set SECTION=Release
	@set INPUTDIRECTORY=./
	@doxygen
	@findstr /v "index.hhk" $(INTDIR)\index.hhp > $(INTDIR)/release.hhp
	-1 @hhc.exe $(INTDIR)/release.hhp

$(INTDIR)/$(DEVFILENAME).chm : $(DOXYGEN_SOURCES) ../*.cpp ../*.h
	@set TARGETFILENAME=$(@F)
	@set SECTION=Develop
	@set INPUTDIRECTORY=./ ../
	@doxygen
	-1 @hhc.exe $(INTDIR)/index.hhp
