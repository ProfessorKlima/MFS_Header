# MFS_Header
Header Datei für das MultiFunctionShield

Nachdem man schieberegister.c und .h dem Projekt hinzugefügt hat muss man nur mehr (in main)
#include "schieberegister.h"
einfügen, sowie init_sr_display() aufrufen.
Das Display setzt man mit update_sr_display().

ACHTUNG: TIMER2 WIRD DURCH DIE FUNKTION BLOCKIERT!
