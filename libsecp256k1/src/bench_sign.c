/**********************************************************************
 * Copyright (c) 2014 Pieter Wuille                                   *
 * Distributed under the MIT software license, see the accompanying   *
 * file COPYING or http://www.opensource.org/licenses/mit-license.php.*
 **********************************************************************/

#include "include/secp256k1.h"
#include "util.h"
#include "bench.h"

typedef struct {
    lw_secp256k1_context* ctx;
    unsigned char msg[32];
    unsigned char key[32];
} bench_sign;

static void bench_sign_setup(void* arg) {
    int i;
    bench_sign *data = (bench_sign*)arg;

    for (i = 0; i < 32; i++) {
        data->msg[i] = i + 1;
    }
    for (i = 0; i < 32; i++) {
        data->key[i] = i + 65;
    }
}

static void bench_sign_run(void* arg, int iters) {
    int i;
    bench_sign *data = (bench_sign*)arg;

    unsigned char sig[74];
    for (i = 0; i < iters; i++) {
        size_t siglen = 74;
        int j;
        lw_secp256k1_ecdsa_signature signature;
        CHECK(lw_secp256k1_ecdsa_sign(data->ctx, &signature, data->msg, data->key, NULL, NULL));
        CHECK(lw_secp256k1_ecdsa_signature_serialize_der(data->ctx, sig, &siglen, &signature));
        for (j = 0; j < 32; j++) {
            data->msg[j] = sig[j];
            data->key[j] = sig[j + 32];
        }
    }
}

int main(void) {
    bench_sign data;

    int iters = get_iters(20000);

    data.ctx = lw_secp256k1_context_create(lw_secp256k1_CONTEXT_SIGN);

    run_benchmark("ecdsa_sign", bench_sign_run, bench_sign_setup, NULL, &data, 10, iters);

    lw_secp256k1_context_destroy(data.ctx);
    return 0;
}
