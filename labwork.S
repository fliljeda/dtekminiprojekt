  # labwork.S
  # Written 2015 by F Lundevall
  # Modified 2015 by F Liljedahl and A Manske
  # Skeleton file for IS1200/IS1500 lab 1 2015-2016.
  # The macros PUSH and POP are in the public domain.
.data
.macro	PUSH reg
	addi	$sp,$sp,-4
	sw	\reg,0($sp)
.endm

.macro	POP reg
	lw	\reg,0($sp)
	addi	$sp,$sp,4
.endm

  # You can add your own code here
  #

.global enable_interrupt
.text

enable_interrupt:
	PUSH $ra
	ei
	POP $ra
	jr $ra
	nop
