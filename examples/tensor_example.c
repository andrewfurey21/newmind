#include "../include/tensor.h"
#include "stdio.h"
#include "stdlib.h"
#include "assert.h"
#include <stdint.h>
#include <time.h>

// TODO:
// encapsulating ops with functions is broken, i dont think gradients flow correcly.
// check summing with (2, 1, 1) or something with ones
// get name of linspace/arange correct
// get this working correctly, compare with proper tinygrad/pytorch impl
// variable shapes etc.
// add to hl_ops or something.
// need to free stuff in function if not being used later.
tt* linear_layer(tt* a, tt* b) {
    ttuple* reshape_a_shape = ttuple_build(3, 1, 4, 3);
    tt* reshape_a = tt_reshape(a, reshape_a_shape);

    ttuple* reshape_b_shape = ttuple_build(3, 5, 4, 1);
    tt* reshape_b = tt_reshape(b, reshape_b_shape);

    tt* expand_a = tt_expand(reshape_a, 0, 5);
    tt* expand_b = tt_expand(reshape_b, 2, 3);

    tt* mul = tt_mul(expand_a, expand_b);

    tt* dot = tt_sum(mul, 1);

    ttuple* reshaped_dot_shape = ttuple_build(2, 5, 3);
    tt* reshaped_dot = tt_reshape(dot, reshaped_dot_shape);

    tt* dot_sum = tt_sum(reshaped_dot, -1);

    return dot_sum;
}

tt* flatten(tt* input, int start_dim) {
    assert(start_dim >= 0 && start_dim < input->view->shape->size);
    ttuple* new_shape = ttuple_zeros(start_dim+1);
    uint64_t end = 1;
    for (int i = 0; i < input->view->shape->size; i++) {
        if (i >= start_dim) {
            end *= input->view->shape->items[i];
        } else {
            new_shape->items[i] = input->view->shape->items[i];
        }
    }
    new_shape->items[start_dim] = end;
    tt* flattened = tt_reshape(input, new_shape);
    return flattened;
}

int main(void) {
    srand(time(NULL));

    ttuple* shape = ttuple_build(4, 3, 2, 4, 4);
    tt* input = tt_linspace(shape, 0, 3*2*4*4, true);
    tt* pool = tt_maxpool2d(input, 2);
    tt* sum = tt_sum(pool, -1);

    tgraph* net = tgraph_build(sum);
    tgraph_zeroed(net);
    tgraph_backprop(net);

    tgraph_print(net, false, true);
}
