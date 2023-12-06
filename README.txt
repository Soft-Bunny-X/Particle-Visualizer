To run the simulation, either open up the solution file, compile and run if visual studio is already installed on the machine.

Or direct through /x64/Release folder and run the "Particle Visualizer.exe" directly. 

Files used to configure the fields and particles are stored in "Initilazation Data" folder


========================================

Instruction on Entering The Field Configuration:

* Multiplication must be explicitly expressed. For examply, product of two variables written as 'xy' is invalid. Instead, write it as 'x*y'.

* Only three functions are supported in the current version: cos(), sin(), and ln(). Inverse trig functions are not yet supported.

* When expressing square root, express it in the power form. For example, 'x^(1/2)'.

* Common constants in math and science are not supported. Express them explicitly as numbers with desired accuracy. For example, instead of writting 'pi', you should write '3.1415926'.

* Only 3 variables are available: x, y, and z. They represent the components of the location in Cartesian coordinate. When expressing them, both lowercase and uppercase are valid.

* Make sure to enclose function argument with parenthesis. For example, 'cosx' would be invalid. Instead, write 'cos(x)'.

* When consecutive exponentials appear, the order of operation is from left to right. For example, '2^3^4 = (2^3)^4'.

* Complex number is not allowed.

========================================

The program requires support for OpenGL.

Make sure the "Initialization Data" and "Shaders" folders are intact and "glew32.dll" file is in the root folder.

The recommended selection for program queries are "Y/N/Y".

The field configurations should be modified through the txt files in the "Initialization Data" folder.

