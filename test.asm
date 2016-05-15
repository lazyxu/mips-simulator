addi   $t0, $zero, 1
addi   $t1, $zero, 22
sllv    $s0, $t0, $t1 ＃START_PC
addi   $s1, $s0, 20 #循环次数

loop:
lw    $t1,0($s0)
xori  $t1, $t1, 0x15a
sw    $t1,0($s0)
lb    $t1,2($s0)
addi  $t1, $t1, 2
sb    $t1,2($s0)
addi  $s0, $s0, 4 #地址
beq   $s0, $t2, end
j loop

end:
addi  $s0, $s1, 1