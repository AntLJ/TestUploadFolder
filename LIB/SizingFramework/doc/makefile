# DOXYGEN_SOURCES = make.bat makefile doxyfile *.dox img/* ../*.rc
DOXYGEN_SOURCES = make.bat makefile doxyfile *.dox img/*

Release : $(INTDIR)/$(RELEASEFILENAME).chm
Develop : $(INTDIR)/$(DEVFILENAME).chm

$(INTDIR)/$(RELEASEFILENAME).chm : $(DOXYGEN_SOURCES)
	@set TARGETFILENAME=$(@F)
	@set SECTION=Release
	@set INPUTDIRECTORY=./
	@doxygen
	@findstr /v "index.hhk" $(INTDIR)\index.hhp > $(INTDIR)/release.hhp
	-1 @hhc.exe $(INTDIR)/release.hhp

$(INTDIR)/$(DEVFILENAME).chm : $(DOXYGEN_SOURCES) ../*.cpp ../*.h ../include/*.h ../test/ATLTestApp/*.cpp ../test/ATLTestApp/*.h ../test/MFCTestApp/*.cpp ../test/MFCTestApp/*.h
	@set TARGETFILENAME=$(@F)
	@set SECTION=Develop
	@set INPUTDIRECTORY=./ ../ ../include ../test/ATLTestApp ../test/MFCTestApp
	@doxygen
	-1 @hhc.exe $(INTDIR)/index.hhp
	@copy $(INTDIR)\index.hhc $(INTDIR)\index_u.hhc
	@iconv -f UTF-8 -t CP932 $(INTDIR)/index_u.hhc > $(INTDIR)/index.hhc
	-1 @hhc.exe $(INTDIR)\index.hhp
