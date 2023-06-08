////////////////////////
//                    //
// Project Submission //
//                    //
////////////////////////

// Partner1: Felipe Bailon, A15532787
// Partner2: none

////////////////////////
//                    //
//       main         //
//                    //
////////////////////////

    lda x0, array
    lda x1, arraySize
	ldur x1, [x1, #0]
    bl printList
    
    lda x0, array
    lda x1, arraySize
    ldur x1, [x1, #0]
    //ADDI X2, XZR, #0
    //ADDI X3, XZR, #4
    bl quicksort       

    lda x0, array
    lda x1, arraySize
    ldur x1, [x1, #0]
    bl printList
	stop

////////////////////////
//                    //
//       swap         //
//                    //
////////////////////////
swap:
    // x0: base address
    // x1: index 1
    // x2: index 2
    // Swap the elements at the given indices in the list

    LSL X10, X1, #3         // Converting the index i value into bits so that it can select the correct part of the array
    LSL X11, X2, #3         // Converting the index j value into bits so that it can select the correct part of the array
    ADD X10, X0, X10        // Storing the array element of i into a register  
    ADD X11, X0, X11        // Storing the array element of j into a register 
    LDUR X12, [X10, #0]     // Loading the values that the index points to from the array into a register
    LDUR X13, [X11, #0]     // Loading the values that the index points to from the array into a register
    STUR X13, [X10, #0]     // Storing the value of i into the address j
    STUR X12, [X11, #0]     // Storing the value of j into the address of i

	
	br lr 
    
////////////////////////
//                    //
//     partition      //
//                    //
////////////////////////
partition:
    // x0: base address
    // x1: The number of integers in the (sub)list
    // x2: Leftmost index to be partitioned in this iteration
    // x3: Rightmost index to be partitioned in this iteration
    // Return:
    // x0: The final index for the pivot element
    // Separate the list into two sections based on the pivot value
	
    SUBI SP, SP, #64 // Create stack for the recursive part of the project
    STUR FP, [SP, #0] // Saving FP value into SP
    ADDI FP, SP, #56 // Moving FP to top of stack but leaving buffer space between current stack and previous stack
    STUR X0, [FP, #-8] // Storing base address into stack
    STUR X1, [FP, #-16] // Storing register value of X1 into stack to save value
    STUR X2, [FP, #-24] // Storing register value of X2 into stack to save value 
    STUR X3, [FP, #-32] // Storing register value of X3 into stack to save value 
    STUR LR, [FP,#-40]  // Saving LR value into stack
   
    ADD X9, X2, X3 // Finding the value of the left index plus the right index
    LSR X9, X9, #1 // Using LSR to divide the value of X9 by half to set as argument in Swap
    ADDI X1,X9,#0   // Saving value of pivot into X1
    ADDI X2, X3, #0 // Saving the rightmost index into X2 for the swap argument
    bl swap         // Branch to swap function and save next instruction lr to lr register
    
    LDUR X1,[FP, #-16] // Loading the previous value of X1 in the stack into X1
    LDUR X2,[FP, #-24] // Loading the previous value of X2 in the stack into X2
    ADDI x4, X2, #0 // Making register X4 hold the value of I which is set to the left index
    ADDI X5, X2, #0 // Making register X5 hold the value of J which is set to the left index
    LSL X10, X3, #3 // Changing the number of right index into bits
    ADD X7, X10, X0 // Determining the address of the element of the right index
    LDUR X6, [X7, #0] // load value of right most index(our pivot) so that we can start using branch compare commands
////////////////////
//While loop code//
///////////////////
    while: SUBS XZR, X4, X3 // Setting flags for i - right

    B.GE exitloop // exitloop if i is greater or equal to right

    LSL  X11,X4,#3 // Convert i into bits
    ADD X12, X0, X11 // Add bits of i into the base address to get the element address of a[i]
    LDUR X13, [X12, #0] // Load the value at a[i] into temporary register
    //SUBS XZR, X13, X6 Subtract the value of a[i] to the pivot and set flags


    SUBS XZR, X13, X6 // Check first condition: is a[i] < a[right]. Instruction gives a[i]-a[right]
    B.LT execute_if // Execute if statement if a[i] is less than a[right]
    
    SUBS XZR, X13, X6   // Set flags for OR statement in if conditions 
    B.EQ Secondcondition // Checking second condition if a[i] = pivot
    b avoid_ifexecution // Branch that occurs when fist condition fails, which skips executing the if code 
    
    Secondcondition:  ANDI X6, X4, #1   // Checks if i is even or odd
    CBZ X6, execute_if // If x6 contains a 0(meaning even number), the branch to execute_if
    b avoid_ifexecution


    execute_if:        // commands in if-statement body
    ADDI X1, X4, #0     // Switch i into X1
    ADDI X2, X5, #0     // Switch j into X2

    bl swap

    LDUR X1,[FP, #-16] // Loading the previous value of X1 in the stack into X1
    LDUR X2,[FP, #-24] // Loading the previous value of X2 in the stack into X2
    ADDI X5, X5, #1 // Increment j by 1
    
    avoid_ifexecution:  // Avoids executing if-statement body if no conditions are met
    ADDI x4, x4, #1 //Increment i by 1
    b while
    
    exitloop: // exit loop label once While condition is not met anymore
///////////////////////////////
//Swapping pivot and j value///
///////////////////////////////
    ADDI X1, X5, #0 // Putting value of j into X1 arguement
    ADDI X2, X3, #0 // Putting value of the right index into X2 arguement
    bl swap    // Swap j and right index elements

    LDUR X1,[FP, #-16] // Loading the previous value of X1 in the stack into X1
    LDUR X2,[FP, #-24] // Loading the previous value of X2 in the stack into X2
    
////////////////////////////////
//Last if conditions and codes//   
////////////////////////////////
    LSR x10, X1, #2 // dividing the size by 4
    SUBS XZR, X5, X10 // setting flags for j-(size divided by 4)
    B.LT returnj+1partition // send to label to execute first if condition
    
    SUB X11, X1, X10 // Subracting size from size divided by 4
    SUBS XZR, X5, X11 // Setting flags to check if j is greater than or equal to X11
    B.GE returnj-1partition

    b else_command

    returnj+1partition:
    ADDI X5, X5, #1 // Add 1 to j for new parameters of Partition
    ADDI X2, X5, #0 // Save j+1 value into X2(left index) parameter
    LDUR X3,[FP, #-32]
    bl partition  // branch to partition with new parameters
    b finish

    returnj-1partition: 
    SUBI X3, X5, #1 // Subtract 1 to j for new parameter of Partition
    //ADDI X2, X5, #1 // Save j-1 value into X2(left index) parameter
    LDUR X2[FP, #-24] // Load left value into x2 in case of inconsistency
    bl partition
    b finish

    else_command:   // Return j statement
    ADDI X0, X5, #0 // Save j value into return register X0
    
    finish:
    LDUR LR, [FP,#-40]  // Restoring old LR value
    LDUR FP, [SP, #0] // Restore old value of the frame pointer
     
    ADDI SP,SP, #64 // Deallocate stack frame

	br lr

////////////////////////
//                    //
//     quickSort      //
//                    //
////////////////////////
quickSort:    
    // x0: base address
    // x1: The number of integers in the (sub)list
    SUBI SP, SP, #48    // Allocating space in stack
    STUR FP, [SP, #0]   // Saving old frame pointer
    ADDI FP, SP, #40    // Moving frame pointer to top of stack and saving 8bits for buffer
    STUR X0, [FP, #-8]  // Saving base address into stack
    STUR X1, [FP, #-16] // Saving x1 into stack just in case
    STUR LR, [FP, #-24]  // Save old LR value to return to previous instruction
    SUBIS XZR, X1, #1   // Setting flags to check if size is greater than 1
    B.GT quicksort_commands // If size is greater than 1, execute if body instructions
    b end_quicksort // If size is equal to or less than 1, end quicksort

    quicksort_commands: // label to execute quicksort
    ADDI X2, XZR, #0    //Set third parameter to 0, left index = to beginning of array
    SUBI X3, X1, #1     // Set fourth parameter to size - 1 for right index
    bl partition        // Go to partition with new parameters
    
    ADDI X1, X0, #0     // Save pivot_position to size of new quicksort
    ADDI X11, X0,#0     // Save pivot_position to temp. register before reloading base address into x0
    STUR X11,[FP,#-32]
    LDUR X0, [FP, #-8]  // Restore value of the base address into x0
    bl quicksort
    LDUR X10,[FP,#-32]
    ADDI X12, X10, #1   //Add 1 to pivot
    LSL X13, X12, #3    // Change index of pivot+1 to bits
    ADD X4, X0, X13     // Address of pivot + 1

    LDUR X3,[FP,#-16]   // Loading size into x3
    SUBI X3,X3, #1      // Subtracting size by 1
    SUB X5, X3, X10     // Subtracting size-1-pivot_position
    ADDI X0, X4, #0      //Save address of pivot + 1 into 1st arguement 
    ADD X1, X5, XZR     // Save new size of size-1-pivot
    bl quicksort
    
     end_quicksort:

    LDUR LR, [FP,#-24]  // Restoring old LR value
    LDUR FP, [SP, #0] // Restore old value of the frame pointer
    LDUR X1, [FP,#-16] // Restoring X1 values
    LDUR X0, [FP,#-8]   // Restoring X0 values
    LDUR X11, [FP, #-32] // Restoring x11 values
    ADDI SP, SP, #48 // Deallocate stack frame
 

    br lr
    
////////////////////////
//                    //
//     printList      //
//                    //
////////////////////////
printList:
    // x0: base address
    // x1: length of the array

	mov x2, xzr
	addi x5, xzr, #32
	addi x6, xzr, #10
printList_loop:
    cmp x2, x1
    b.eq printList_loopEnd
    lsl x3, x2, #3
    add x3, x3, x0
	ldur x4, [x3, #0]
    putint x4
    putchar x5
    addi x2, x2, #1
    b printList_loop
printList_loopEnd:    
    putchar x6
    br lr







