//
// Created by sergeyampo on 17.06.2020.
//

#ifndef PARALLEL_FACTORIAL_SRC_NUMBER_PARTITION_H_
#define PARALLEL_FACTORIAL_SRC_NUMBER_PARTITION_H_

struct Interval {
  uint64_t begin;
  uint64_t end;
};

struct Interval* partition_num_to_intervals(const uint64_t*, uint64_t);

#endif //PARALLEL_FACTORIAL_SRC_NUMBER_PARTITION_H_
