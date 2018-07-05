/*
 * Copyright 1992 by Jutta Degener and Carsten Bormann, Technische
 * Universitaet Berlin.  See the accompanying file "COPYRIGHT" for
 * details.  THERE IS ABSOLUTELY NO WARRANTY FOR THIS SOFTWARE.
 */

/*$Header: /home/kbs/jutta/src/gsm/gsm-1.0/inc/RCS/gsm.h,v 1.11 1996/07/05 18:02:56 jutta Exp $*/

#ifndef	LIBGSM_H
#define	LIBGSM_H

#include "common/scummsys.h"

/*
 *	Interface
 */

namespace libgsm {

typedef struct gsm_state * 	gsm;
typedef int16		   	gsm_signal;		/* signed 16 bit */
typedef byte		gsm_byte;
typedef gsm_byte 		gsm_frame[33];		/* 33 * 8 bits	 */

/*
#define	GSM_PATCHLEVEL		18
#define	GSM_MINOR		0
#define	GSM_MAJOR		1
*/

enum gsm_opt {
	GSM_OPT_VERBOSE		= 1,
	GSM_OPT_FAST		= 2,
	GSM_OPT_LTP_CUT		= 3,
	GSM_OPT_WAV49		= 4,
	GSM_OPT_FRAME_INDEX	= 5,
	GSM_OPT_FRAME_CHAIN	= 6
};

gsm  gsm_create 	(void);
void gsm_destroy (gsm S);	

int  gsm_option  (gsm r, gsm_opt opt, int * val);

int  gsm_decode  (gsm s, gsm_byte * c, gsm_signal * target);

} // namespace libgsm

#endif	/* GSM_H */
