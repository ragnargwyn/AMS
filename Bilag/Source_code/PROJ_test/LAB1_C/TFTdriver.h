extern int vsp;
extern int invvsp;

void DisplayInit();
void AllPixelsOff();
void AllPixelsOn();
void DisplayOff();
void DisplayOn();
void SleepOut();
void MemoryAccessControl(unsigned char parameter);
void InterfacePixelFormat(unsigned char parameter);
void WritePixel(unsigned char Red, unsigned char Blue, unsigned char Green);
void SetColumnAddress(unsigned int Start, unsigned int End);
void SetPageAddress(unsigned int Start, unsigned int End);
void MemoryWrite();
void DrawLine(unsigned int StartX, unsigned int StartY, unsigned int Width, unsigned char Red, unsigned char Green, unsigned char Blue);
void InitScrolling();
void MoveDisplay();
void UpdateDisplayPointer(unsigned int width);