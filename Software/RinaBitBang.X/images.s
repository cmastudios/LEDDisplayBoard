# link images as raw GRB arrays straight into flash memory
# we have 270kB to play with on this overspecced CPU, each image weighs 660b
# so we can fit over 400 images if we need
	.section .rodata

	.global image_face
	.type   image_face, %object
	.align  4
image_face:
	.incbin "images/face.bin"

	.global image_cse467
	.type   image_cse467, %object
	.align  4
image_cse467:
	.incbin "images/cse467.bin"

	.global image_washu
	.type   image_washu, %object
	.align  4
image_washu:
	.incbin "images/washu.bin"

	.global image_error_battery
	.type   image_error_battery, %object
	.align  4
image_error_battery:
	.incbin "images/lowbattery.bin"

	.global image_error_wireless
	.type   image_error_wireless, %object
	.align  4
image_error_wireless:
	.incbin "images/wirelesserror.bin"
