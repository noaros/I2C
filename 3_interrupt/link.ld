SECTIONS {	
	.text : {
		LONG(0x20030000)
		LONG(mystart | 1)
		. = . + 13 * 4;
		LONG(systick | 1)
		. = . + 91 * 4;
	} 
}
