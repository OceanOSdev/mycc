.class super libc 
.super java/lang/Object 

.method <init> : ()V 
    .code stack 1 locals 1 
L0:     aload_0 
L1:     invokespecial Method java/lang/Object <init> ()V 
L4:     return 
L5:     
    .end code 
.end method 

.method public static putchar : (I)I 
    .code stack 2 locals 1 
L0:     getstatic Field java/lang/System out Ljava/io/PrintStream; 
L3:     iload_0 
L4:     i2c 
L5:     invokevirtual Method java/io/PrintStream print (C)V 
L8:     iload_0 
L9:     ireturn 
L10:    
    .end code 
.end method 

.method public static getchar : ()I 
    .code stack 1 locals 0 
L0:     getstatic Field java/lang/System in Ljava/io/InputStream; 
L3:     invokevirtual Method java/io/InputStream read ()I 
L6:     ireturn 
L7:     
    .end code 
    .exceptions java/io/IOException 
.end method 
.end class 
