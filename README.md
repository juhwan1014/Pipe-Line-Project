# Pipe-Line-Project 

This project is an extension of the MIPS Simulator Project, which is a program that convert input files in binary form into multiple-cycle MIPS by referring to the MIPS green sheet. <br />
The details are briefly described as follows:<br /><br />

'Fetch' -> Using 'fopen', 'fread' to pull out a single line of input binary files to retrieve the instruction value. <br /><br />
'Decode' -> Pull out the op-code first, divide it to fit each type, pull out the necessary ingredients and store them in EK. <br /><br />
'Execute' -> ALU values are calculated and stored separately for reading from the memory access stage.<br /><br />
'Memory Access' -> Collects the instructions of the memory values that are used or changed, and stores the result values to be sent to the final Write back stage using the result values taken from Excute stage.<br /><br />
'Write Back' -> update the final register value.<br /><br />
