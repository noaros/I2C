SECTIONS {	
	.text : {
		LONG(0)
		LONG(mystart | 1)
		. = . + 420;
	} 
}
