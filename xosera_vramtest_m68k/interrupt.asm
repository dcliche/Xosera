; *************************************************************
; Copyright (c) 2021 roscopeco <AT> gmail <DOT> com
; *************************************************************
;
        section .text                     ; This is normal code

        include "../xosera_m68k_api/xosera_m68k_defs.inc"

install_intr::
                movem.l D0-D7/A0-A6,-(A7)

                or.w    #$0200,SR               ; disable interrupts

                move.l  #XM_BASEADDR,A0         ; get Xosera base addr
                move.b  #$0F,D0                 ; all interrupt source bits
                move.b  D0,XM_TIMER+2(A0)       ; clear out any prior pending interrupts
                move.w  #$0800,D0               ; vsync interrupt
                movep.w D0,XM_INT_CTRL(A0)      ; enable VSYNC interrupt

                move.l  #Xosera_intr,$68        ; set interrupt vector
                and.w   #$F0FF,SR               ; enable interrupts

                movem.l (A7)+,D0-D7/A0-A6
                rts

remove_intr::
                movem.l D0-D7/A0-A6,-(A7)

                move.l  #XM_BASEADDR,A0         ; get Xosera base addr
                moveq.l #$000F,D0               ; disable interrupts, and clear pending
                movep.w D0,XM_INT_CTRL(A0)      ; enable VSYNC interrupt
                move.l  $60,D0                  ; copy spurious int handler
                move.l  D0,$68

                movem.l (A7)+,D0-D7/A0-A6
                rts

; interrupt routine
Xosera_intr:
                movem.l D0-D1/A0,-(A7)          ; save minimal regs

                move.l  #XM_BASEADDR,A0         ; get Xosera base addr
                movep.w XM_WR_XADDR(A0),D1      ; save aux_addr value

                move.b  XM_INT_CTRL+2(A0),D0       ; read pending interrupts
                move.b  D0,XM_INT_CTRL+2(A0)       ; acknowledge and clear all interrupts

                ; NOTE: could check D0 bits [3:0] for
                ;       interrupt sources, but for now
                ;       just assume it is vsync [3]

                move.w  #XR_COLOR_ADDR,D0       ; set color entry #0
                movep.w D0,XM_WR_XADDR(A0)

                move.w  NukeColor,D0            ; load color
                bmi.s   NoNukeColor             ; if color < 0, skip color cycle

                addq.l  #1,D0                   ; cycle color
                and.w   #$0FFF,D0               ; clear non-RGB bits
                move.w  D0,NukeColor            ; save color
                movep.w D0,XM_XDATA(A0)         ; to NukeColor

NoNukeColor:    add.l   #1,XFrameCount          ; increment frame counter

                movep.w D1,XM_WR_XADDR(A0)      ; restore aux_addr
                movem.l (A7)+,D0-D1/A0          ; restore regs
                rte

        section .data

NukeColor::     dc.w    $0000
XFrameCount::   dc.l    $00000000
