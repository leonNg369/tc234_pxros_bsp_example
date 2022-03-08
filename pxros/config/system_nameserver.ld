SECTIONS
{
	.nameSrv :
	{
		. = ALIGN(8);
		PxNameServerSpaceBase = .;
		. += _NAMESERVER_SIZE_;
		PxNameServerSpaceEnd = .;
	} > CORE_SYM(BSS)
	
	CORE_SEC(.clear_sec) :
	{
	   	LONG(0 + ADDR(.nameSrv)); LONG(SIZEOF(.nameSrv));
	} > RODATA_MEM
}

