.386
.model flat,stdcall
option casemap:none

include	windows.inc
include	msvcrt.inc
includelib	msvcrt.lib

get_rand	PROTO	C
moveCursor	PROTO	C	x:dword,	y:dword
sleep		PROTO	C
turn		PROTO	C

.data
;��20����30
h		equ	20
w		equ	40
mp	dword	h	dup(w	dup(0))	;��ͼ

direction	dword	?	;1234����������
food_x	dword	?
food_y	dword	?
old_tail	dword	0
old_head	dword	0
new_head	dword	0
max_num		dword	?
game_over	dword	0	;��Ϊ1����Ϸ����

sz_int	byte	"%d ", 0
sz_char	byte	"%c", 0

.code



startUp	proc	C
	;���Ʊ߿�
	;invoke	crt_printf,	offset	sz_int,	mp[30 * 20 * 4 - 4]
	mov	ecx,	h
	.while	ecx
		dec	ecx
		mov	eax,	w*4
		mul	ecx
		mov	mp[eax + 0*4],		-1
		mov	mp[eax + (w-1)*4],	-1
	.endw
	mov	ecx,	w
	.while	ecx
		dec	ecx
		mov	mp[0*4 + ecx*4],				-1
		mov	mp[(h-1)*w*4 + ecx*4],	-1
	.endw
	;invoke	crt_printf,	offset	sz_int,	mp[0 * 4]			;���Ͻ�
	;invoke	crt_printf,	offset	sz_int,	mp[(w-1) * 4]		;���Ͻ�
	;invoke	crt_printf,	offset	sz_int,	mp[(h-1) * w * 4]	;���½�
	;invoke	crt_printf,	offset	sz_int,	mp[h * w * 4 - 4]	;���½�
	;invoke	crt_printf,	offset	sz_int,	mp[h * w * 4]		;�������
	;invoke	crt_printf,	offset	sz_int,	mp[w * 4]			;�ڶ�����һ
	;invoke	crt_printf,	offset	sz_int,	mp[w * 4 + 4]		;�ڶ������
	
	;��ͷ
	mov	mp[h/2*w*4 + w/2*4],	1

	;����
	mov	ecx,	2
	.while	ecx < 6
		mov	eax,	ecx
		dec	eax
		mov	mp[h/2*w*4 + w/2*4 + eax*4],	ecx
		inc	ecx
	.endw

	;�ƶ�����
	mov	direction,	4

	;ʳ��
	invoke	get_rand
	mov	mp[eax*4],	-2
	ret
startUp	endp

show	proc	C
	invoke	moveCursor,	offset 0,	offset 0

	mov	ecx,	h
	.while	ecx
		dec	ecx	;from h-1 to 0
		mov	edx,	w
		.while	edx
			dec	edx	;from w-1 to 0
			push	ecx
			push	edx

			mov	eax,	w*4
				push	edx
			mul	ecx
				pop		edx
			mov	eax,	mp[eax + edx*4]
			; now, the register "eax" is the value of the corresponding point in the mp
			.if eax == 0				
				invoke	crt_printf,	offset	sz_char,	" "
			.elseif	eax == -2
				invoke	crt_printf,	offset	sz_char,	"$"
			.elseif eax == -1
				invoke	crt_printf,	offset	sz_char,	"#"
			.elseif	eax == 1
				invoke	crt_printf,	offset	sz_char,	"@"
			.elseif eax > 1
				invoke	crt_printf,	offset	sz_char,	"*"
			.endif
			
			pop		edx
			pop		ecx
		.endw
		push	ecx
		invoke	crt_printf,	offset	sz_char,	0ah
		pop	ecx
	.endw

	ret
show	endp

moveSnake	proc	C

	;��++
	mov	ecx,	h
	.while	ecx
		dec	ecx	;from h-1 to 0
		mov	edx,	w
		.while	edx
			dec	edx	;from w-1 to 0
			push	ecx
			push	edx

			mov	eax,	w*4
				push	edx
			mul	ecx
				pop		edx
			mov	ebx,	mp[eax + edx*4]
			; now, the register "ebx" is the value of the corresponding point in the mp
			cmp	ebx,	0
			jg	Positive1
			jmp	Negative1
			Positive1:
				inc	ebx
				mov	mp[eax + edx*4],	ebx
			Negative1:
			pop		edx
			pop		ecx
		.endw
		push	ecx
		pop	ecx
	.endw

	;Ѱ��old_head��old_tail
	mov	max_num,	0
	mov	ecx,	h
	.while	ecx
		dec	ecx	;from h-1 to 0
		mov	edx,	w
		.while	edx
			dec	edx	;from w-1 to 0
			push	ecx
			push	edx

			mov	eax,	w*4
				push	edx
			mul	ecx
				pop		edx
			mov	ebx,	mp[eax + edx*4]
			; now, the register "ebx" is the value of the corresponding point in the mp
			cmp	ebx,	0
			jg	Positive2
			jmp	Negative2
			Positive2:
					push	ebx
				.if ebx == 2
					mov	ebx,	eax
					mov	eax,	4
					mul	edx
					add	ebx,	eax
					mov	old_head,	ebx					
				.endif
					pop		ebx
				.if max_num < ebx
					mov	max_num,	ebx
					mov	ebx,	eax
					mov	eax,	4
					mul	edx
					add	ebx,	eax
					mov	old_tail,	ebx
				.endif
			Negative2:
			pop		edx
			pop		ecx
		.endw
		push	ecx
		pop	ecx
	.endw

	;�ƶ��ߣ����new_head
	mov	eax,	old_head
	.if	direction == 1
		add	eax,	w*4
	.elseif	direction == 2
		sub	eax,	w*4
	.elseif	direction == 3
		add	eax,	4				
	.elseif	direction == 4
		sub	eax,	4
	.endif
	mov	new_head,	eax

	;�Ƿ�Ե�ʳ��
	mov	ebx,	new_head
	mov	eax,	mp[ebx]
	.if	eax == -2
		mov	mp[ebx],	0
		invoke	get_rand
		mov	mp[eax*4],	-2
	.else
		mov	ebx,	old_tail
		mov	mp[ebx],	0
	.endif

	;��Ϸ�Ƿ����
	mov	ebx,	new_head
	mov	eax,	mp[ebx]
	.if	eax > 0
		mov	game_over,	1
	.else
		mov	mp[ebx],	1
	.endif

	;�����ߵ��ٶ�
	invoke	sleep

	ret
moveSnake	endp

updateWithoutInput	proc	C
	invoke	moveSnake
	ret
updateWithoutInput	endp

updateWithInput	proc	C
	invoke	turn
	.if	eax
		mov	direction,	eax
	.endif
	ret
updateWithInput	endp

main	proc	C
	invoke	startUp
	.while TRUE
		invoke	updateWithInput
		invoke	updateWithoutInput
		invoke	show
		.if	game_over
			.break
		.endif
		invoke	sleep
	.endw
	ret
main	endp
end	
