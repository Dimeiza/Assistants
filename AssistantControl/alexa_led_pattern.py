#!/usr/bin/env python

def AlexaLedPattern(show):
    basis = [0] * 3 * 3
    basis[2] = 1
    basis[5] = 1
    basis[8] = 3
    
    return basis