.global contextJump
.type contextJump, @function
contextJump:
    ld ra, 0(a0)
    ld sp, 8(a0)

    ret