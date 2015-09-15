#! /usr/bin/sh

wffanalyze 'A||B||C||D||E||F||G||H||I||J||K||L||M||N||P||O||Q||R||S||T||U||V||W||X||Y||Z||AA||AB||AC||AD||AE||AF||AG'

wffanalyze 'A|B&C&D'

wffanalyze '((P||Q)&&R)->P'

wffanalyze

wffanalyze '123()*(#$'
