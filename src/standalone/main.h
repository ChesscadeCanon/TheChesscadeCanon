#pragma once
#define MILLISECONDS_DIFFERENCE(__from_time__, __to_time__) ((Time)(1000.0 * ((__to_time__).tv_sec - (__from_time__).tv_sec) + ((__to_time__).tv_nsec / 1000000 - (__from_time__).tv_nsec / 1000000)))
