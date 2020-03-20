unsigned char *LoadBitmapFile(char *filename)
{
    BITMAPINFOHEADER *bitmapInfoHeader = (BITMAPINFOHEADER *)malloc(sizeof(BITMAPINFOHEADER));
    FILE *filePtr;
    BITMAPFILEHEADER bitmapFileHeader;

    filePtr = fopen(filename,"rb");
    if (filePtr == NULL) {
        return NULL;
	}

    fseek (filePtr, 0, SEEK_END);   // non-portable
    long size=ftell (filePtr);
    size -= sizeof(BITMAPFILEHEADER);

    unsigned char *bitmap = (unsigned char *)malloc(size);

    fseek(filePtr, sizeof(BITMAPFILEHEADER), SEEK_SET);

    fread(bitmap, size,1,filePtr);
    fclose(filePtr);
	
    return bitmap;
}
