---
title: hip.bw~

description: Highpass butterworth filter

categories:
- object

pdcategory:

arguments:
- description: cutoff in hertz
  type: float
  default: 0
- description: order of the filter from 2 to 100
  type: float
  default: 2

inlets:
  1st:
  - type: signal
    description: the signal to be filtered
  2nd:
  - type: float
    description: cutoff frequency in hertz
  3rd:
  - type: float
    description: order of the filter

outlets:
  1st:
  - type: signal
    description: the filtered signal

draft: false
---

[hip.bw~] is a highpass butterworth filter abstraction based on [biquads~]. You can specify the cuttoff frequency and the order of the filter (from 2nd order to 100th order).
