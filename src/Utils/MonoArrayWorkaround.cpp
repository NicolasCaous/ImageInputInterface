// Unity3D Array Workaround
typedef void* MonoArray;

// Defining a byte
typedef unsigned char byte;

enum { kMonoArrayOffset = 0 };

/*return the i element of the array*/
template<class T>
T GetMonoArrayElement (MonoArray array, int i)
{
    char* raw = kMonoArrayOffset + i * sizeof (T) + (char*)array;
    return *(T*)raw;
}

/*insert elem in the position i of the array*/
template<class T>
void SetMonoArrayElement (MonoArray array, int i, T elem)
{
    char* raw = kMonoArrayOffset + i * sizeof(T) + (char*)array;
    *(T*)raw = elem;
}
