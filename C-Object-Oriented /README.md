# Programming in C with objects(oriented-object programming)
Writes oriented-object polyphormic methods and objects with adapted "prototype oop"(like lua...) system in C language. 

Example:
```c
  class Integer = Template_Class(
    
    constructor({
      set(value, y, int);
    }, args(int y));
    
    create_method(mul, message(void, {
      set(value, get(int, value)*i, int);
    }, args(int i))) ;
    
    create_method(print, message(void, {
      printf("%d\n", get(int, value));
    }));
    
    object(value, type(int));
  );

  call_constructor(Integer, args(2));
  method(void, Integer, mul, args(10));
  method(void, Integer, print); // => prints value 20
  ```
  # Compiling
You should use gcc compiler using the flag -std=c99 this code depend of the P99 library so extract in same folder of this. 
P99 is able in: http://p99.gforge.inria.fr/

I can use this in real prodution project? You can but it is higly recommend not to do this.

  
