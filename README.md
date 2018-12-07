# ARM_Project_Compiler
# Project Team members- Richa Gupta(MT2017513),Tridib Sarma(MT2017525),Vamsi Krishna G(MT2017526)

# Problem Statement
To write a C code which will convert the input expression to postfix notation, as well as creates an assembly code which is compatible with cortex M4


# Sample Input 
2*(4+5)-10/2

# postfix expression generated
245+*102/-

# output assembly code generated

	AREA appcode, CODE, READONLY
	EXPORT __main
	ENTRY
__main FUNCTION
		MOV R0,#5
		MOV R1,#4
		ADD R0,R1
		MOV R1,#2
		MUL R0,R1
		MOV R1,#10
		MOV R2,#2
		SDIV R1,R2
		SUB R0,R1
stop B stop
	ENDFUNC
	END

# Power Point illustrating the working of code
https://iiitborg-my.sharepoint.com/:p:/g/personal/tridib_sarma_iiitb_org/EW1OI9xQO7FJj6VKmNLKfR4BJdJ9A71itUq47Rw8oZ6TNQ?e=iY4J7A
