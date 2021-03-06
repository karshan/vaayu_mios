// $Id: seq_live.h 2048 2014-09-06 12:37:09Z tk $
/*
 * Header file for live routines
 *
 * ==========================================================================
 *
 *  Copyright (C) 2008 Thorsten Klose (tk@midibox.org)
 *  Licensed for personal non-commercial use only.
 *  All other rights reserved.
 * 
 * ==========================================================================
 */

#ifndef _SEQ_LIVE_H
#define _SEQ_LIVE_H

/////////////////////////////////////////////////////////////////////////////
// Global definitions
/////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////
// Global Types
/////////////////////////////////////////////////////////////////////////////

typedef union {
  u32 ALL;
  struct {
    s8 OCT_TRANSPOSE;
    u8 VELOCITY;
    u8 FORCE_SCALE:1;
    u8 FX:1;
    u8 KEEP_CHANNEL:1;
  };
} seq_live_options_t;


/////////////////////////////////////////////////////////////////////////////
// Prototypes
/////////////////////////////////////////////////////////////////////////////

extern s32 SEQ_LIVE_Init(u32 mode);

extern s32 SEQ_LIVE_PlayEvent(u8 track, mios32_midi_package_t p);
extern s32 SEQ_LIVE_AllNotesOff(void);


/////////////////////////////////////////////////////////////////////////////
// Export global variables
/////////////////////////////////////////////////////////////////////////////

extern seq_live_options_t seq_live_options;

#endif /* _SEQ_LIVE_H */
