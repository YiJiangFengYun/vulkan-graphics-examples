#include <stdlib.h>
#include <stdio.h>

FILE* open_or_exit(const char* fname, const char* mode)
{
    FILE* f;
    errno_t err = fopen_s(&f, fname, mode);
    if (err != 0) {
    	perror(fname);
    	exit(1);
    }
    return f;
}

int main(int argc, char** argv)
{
    if (argc < 3) {
    	fprintf(stderr, "Please specify namespace name and variable name to save embed value in code, input file to embed and out file to create code.");
    	return 4;
    }    
    const char * namespaceName = argv[1];
    const char * variableName = argv[2];
    const char * inputFilePath = argv[3];
    const char * outputFilePath = argv[4];
    FILE* in = open_or_exit(inputFilePath, "rb");    
    FILE* out = open_or_exit(outputFilePath, "w");   
    size_t lSize;
    unsigned char * buffer;
    size_t result;
    fseek(in, 0, SEEK_END);
    lSize = static_cast<size_t>(ftell(in));
    rewind(in);    
    // allocate memory to contain the whole file:
    buffer = (unsigned char*)malloc(sizeof(char)*lSize);
    if (buffer == NULL) 
    {
      fprintf(stderr, "Memory aloocate error");
      exit(2); 
    }    
    // copy the file into the buffer:
    result = fread(buffer, 1, lSize, in);
    if (result != lSize) 
    { 
      fprintf(stderr, "Reading file error, target size: %d, result size: %d", lSize, result);
      fputs("Reading error", stderr); 
      exit(3); 
    }    
    /* the whole file is now loaded in the memory buffer. */
    size_t linecount = 0;
    size_t i;
		fprintf(out, "#include <stdint.h>\n");
		fprintf(out, "namespace %s {\n", namespaceName);
    fprintf(out, "    const unsigned char %s[] = {\n    ", variableName);
    for (i = 0; i < lSize; ++i) {
    	fprintf(out, "0x%02x, ", *(buffer + i));
    	if (++linecount == 10) { fprintf(out, "\n    "); linecount = 0; }
    }
    if (linecount > 0) fprintf(out, "\n    ");
    fprintf(out, "};\n");
    fprintf(out, "const size_t %sLen = sizeof(%s);\n\n", variableName, variableName);    
		fprintf(out, "} //%s", namespaceName);
    fclose(in);
    fclose(out);
    free(buffer);
    return EXIT_SUCCESS;
}