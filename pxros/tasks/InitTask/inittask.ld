/* ================================================================================================
 * Project:  PXROS-HR BSP example
 *
 * Description
 *    Init Task stack memories placement. 
 *    Implemented in multicore style as Init Task is instantiated on each active core. 
 *
 * History
 *    v1   03.2019   Update to PXROS-HR v7.3.0
 *
 * ================================================================================================
 * Copyright HighTec EDV-Systeme GmbH 1982-2019
 * ==============================================================================================*/

SECTIONS
{
	CORE_SEC(.inittask) :
	{
	    *(CORE_SEC(.inittask.stack))
	} > CORE_SYM(BSS)
}
