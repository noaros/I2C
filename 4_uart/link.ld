SECTIONS {	
	.text : {
		LONG(0x20030000)
		LONG(mystart | 1)
		. = . + 420;
	} 
}
