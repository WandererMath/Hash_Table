#include "hash.cpp"
#include <cstring>
const char path[]="HashTable";
const unsigned int file_size=1200000; //1200k
const int N=20;

//  Returns position to write
//  exit(1) if key exists or file doesn't exist
unsigned int insert(char* key){    
    unsigned int p=fnv1::hash_N(std::string(key), N);
    return p;
}

// Format:
// 4 bytes Total Size: unsigned int
// Key '\0'
// Content
void write2table(char* key, std::string text, unsigned int position){
    FILE* f=fopen(path, "rb+");
    unsigned int h=static_cast<unsigned int>(text.size());
    if(f==NULL){
        printf("File Error!");
        exit(1);
    }
    fseek(f, static_cast<int>(position), SEEK_SET);
    fwrite(&h, 4, 1, f);
    fwrite(key, 1, strlen(key)+1, f);
    for(char c:text){
        fwrite(&c, 1, 1, f);
    }
    fclose(f);
}


// Error Handling
// Closing file first
void Error(FILE* f, char* error_message){
    printf("%s\n", error_message);
    exit(1);
}

void create_file(){
    FILE* fp=fopen(path, "wb");
    const int X=1000;
    char buffer[X];
    for (int i=0; i<X; i++)
        buffer[i]='\0';
    for (int i=0; i<file_size/X; i++){
        fwrite(buffer, 1, X, fp);
    }
    fclose(fp);
}

unsigned int retrieve_position(char* key){
    return fnv1::hash_N(std::string(key), 20);
}

void retrieve_print(FILE* fp, unsigned int length){
    int c;
    //fread(&c, 1, 1, fp);
    //fread(&c, 1, 1, fp);
    for (int i=0; i<static_cast<int>(length); i++){
        fread(&c, 1, 1, fp);
        printf("%c", c);
    }
    printf("\n");
}

// Return true if vacant
bool retrieve(char* key, bool print=true){
    unsigned int length;
    char c=2;
    bool flag=false;
    int i;
    unsigned int hash=retrieve_position(key);
    FILE* fp=fopen(path, "rb");
    fseek(fp, hash, SEEK_SET);
    while (!flag){
        i=0;
        fread(&length, 4, 1, fp);
        if(length==0){
            fclose(fp);
            return true;
        }
        while(true){
            fread(&c, 1, 1, fp);
            if(c!=key[i]){
                while(c!='\0'){
                    fread(&c, 1, 1, fp);
                }
                fseek(fp, static_cast<int>(length), SEEK_CUR);
                break;
            }
            i++;
            if(c=='\0'){
                flag=true;
                if(print)   retrieve_print(fp, length);
                break;
            }
        }
    }
    fclose(fp);
    return false;
}

// Entry of the program
int main(int argc, char** argv){
    // Program Option Key
    std::string text;
    switch (argv[1][0])
    {
        // No space allowed in key
    case 'i':
        printf("Enter the message\n");
        getline(std::cin, text);
        write2table(argv[2], text, insert(argv[2]));
        break;
    case 'c':
        create_file();
        break;
    case 'r':
        if(retrieve(argv[2]))   printf("Key not found!\n");
        break;
    
    default:
        break;
    }
}

