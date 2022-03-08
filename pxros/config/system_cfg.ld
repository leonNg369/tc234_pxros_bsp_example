/* ================================================================================================
 * Project:  PXROS-HR BSP example
 *
 * Description
 *    Placement of PXMcTaskdefault memory class in memory.
 *    Written in multicore style as each core needs its memory class in its memory, but 
 *    placement pattern is the same.
 *
 * History
 *    v1   03.2019   Update to PXROS-HR v7.3.0.
 *
 * ================================================================================================
 * Copyright HighTec EDV-Systeme GmbH 1982-2019
 * ==============================================================================================*/

SECTIONS
{
	/* InitSpec - Task Memory Class (PXMcTaskdefault) */
	CORE_SEC(.taskmemory) :
	{
	    *(CORE_SEC(.taskmemory))
	} > CORE_SYM(BSS)

	/* Clear area in crt0 */
	CORE_SEC(.clear_sec) :
	{
	    LONG(0 + ADDR(CORE_SEC(.taskmemory))); LONG(SIZEOF(CORE_SEC(.taskmemory)));
	} > RODATA_MEM

}
