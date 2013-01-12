#ifndef DATLOG
#define DATLOG

const int kMaxFileSize = 5000;
TFileHandle hFileWriteHandle = NOT_A_HANDLE;
bool bFirstNumberOnLine = true;
TFileIOResult nIoResult = ioRsltSuccess;

bool bOpenWriteTextFile(const string &sFileName, int nFileSize)
{
	bFirstNumberOnLine = true;
	Delete(sFileName, nIoResult);
	OpenWrite(hFileWriteHandle, nIoResult, sFileName, nFileSize);
	return nIoResult == ioRsltSuccess;
}

void closeWriteTextFile()
{
	Close(hFileWriteHandle, nIoResult);
	hFileWriteHandle = NOT_A_HANDLE;
}

void writeNewLine()
{
	WriteText(hFileWriteHandle, nIoResult, "\r\n");
	bFirstNumberOnLine = true;
	return;
}

void writeDelimiterBetweenNumbers()
{
	if (bFirstNumberOnLine)
		bFirstNumberOnLine = false;
	else
		WriteText(hFileWriteHandle, nIoResult, ", ");
	return;
}

void writeIntegerNumber(long nNumber)
{
	string sTemp;
	writeDelimiterBetweenNumbers();
	StringFormat(sTemp, "%d", (long) nNumber);
	WriteText(hFileWriteHandle, nIoResult, sTemp);
	return;
}

#endif
