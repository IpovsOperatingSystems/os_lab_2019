//
// Created by sergeyampo on 17.06.2020.
//
#include <stdint.h>
#include <stdlib.h>
#include "number_partition.h"


/**
 * 
 * @param num - source number which we should partition 
 * @param intervals - amount of intervals. If intervals > num, we'll partition to num intervals.
 * @return struct Interval* - pointer to the array of intervals, which size is intervals or num if intervals > num.
 * @description For example num=7, intervals=3 = [1 ; 2], [3 ; 4] [5 ; 7]. num=3, intervals=4 [1 ; 1], [2 ; 2], [3 ; 3]
 */
struct Interval* partition_num_to_intervals(const uint64_t* num, uint64_t intervals) {
  uint64_t parts = *num / intervals;
  if (parts == 0) {
	intervals = *num;
	parts = 1;
  }
  struct Interval* output_intervals = (struct Interval*)malloc(sizeof(struct Interval) * intervals);

  uint64_t j = 1;
  for (uint64_t i = 0; i < intervals - 1; ++i) {
	output_intervals[i].begin = j;
	output_intervals[i].end = j - 1 + parts;
	j += parts;
  }
  output_intervals[intervals - 1].begin = j;
  output_intervals[intervals - 1].end = *num;

  return output_intervals;
}
