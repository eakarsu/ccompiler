int printf(const char *fmt, ...);

// Test 072: Error propagation chains - errors bubble up through call layers

enum Err {
    OK,
    E_INPUT,
    E_PARSE,
    E_VALIDATE,
    E_PROCESS,
    E_STORE,
    E_COMMIT
};

struct Result {
    int value;
    int error;
    int layer;
};

struct Result make_result(int value, int error, int layer) {
    struct Result r;
    r.value = value;
    r.error = error;
    r.layer = layer;
    return r;
}

char *err_str(int e) {
    if (e == OK) return "OK";
    if (e == E_INPUT) return "E_INPUT";
    if (e == E_PARSE) return "E_PARSE";
    if (e == E_VALIDATE) return "E_VALIDATE";
    if (e == E_PROCESS) return "E_PROCESS";
    if (e == E_STORE) return "E_STORE";
    if (e == E_COMMIT) return "E_COMMIT";
    return "E_UNKNOWN";
}

struct Result layer_input(int raw) {
    if (raw < 0) {
        return make_result(0, E_INPUT, 1);
    }
    return make_result(raw, OK, 1);
}

struct Result layer_parse(int raw) {
    struct Result r = layer_input(raw);
    if (r.error != OK) {
        r.layer = 2;
        return r;
    }
    if (r.value > 1000) {
        return make_result(0, E_PARSE, 2);
    }
    return make_result(r.value * 2, OK, 2);
}

struct Result layer_validate(int raw) {
    struct Result r = layer_parse(raw);
    if (r.error != OK) {
        r.layer = 3;
        return r;
    }
    if (r.value % 7 == 0) {
        return make_result(0, E_VALIDATE, 3);
    }
    return make_result(r.value, OK, 3);
}

struct Result layer_process(int raw) {
    struct Result r = layer_validate(raw);
    if (r.error != OK) {
        r.layer = 4;
        return r;
    }
    int processed = r.value + 100;
    if (processed > 500) {
        return make_result(0, E_PROCESS, 4);
    }
    return make_result(processed, OK, 4);
}

struct Result layer_store(int raw) {
    struct Result r = layer_process(raw);
    if (r.error != OK) {
        r.layer = 5;
        return r;
    }
    return make_result(r.value, OK, 5);
}

struct Result full_pipeline(int raw) {
    struct Result r = layer_store(raw);
    if (r.error != OK) {
        return r;
    }
    return make_result(r.value, OK, 6);
}

int main() {
    int test_inputs[8];
    test_inputs[0] = 5;
    test_inputs[1] = -1;
    test_inputs[2] = 1500;
    test_inputs[3] = 7;
    test_inputs[4] = 250;
    test_inputs[5] = 0;
    test_inputs[6] = 100;
    test_inputs[7] = 3;

    int success_count = 0;
    int fail_count = 0;

    for (int i = 0; i < 8; i++) {
        struct Result r = full_pipeline(test_inputs[i]);
        if (r.error == OK) {
            printf("input %d -> value=%d layer=%d status=%s\n",
                   test_inputs[i], r.value, r.layer, err_str(r.error));
            success_count++;
        } else {
            printf("input %d -> FAIL at layer %d error=%s\n",
                   test_inputs[i], r.layer, err_str(r.error));
            fail_count++;
        }
    }
    // EXPECT: input 5 -> value=110 layer=6 status=OK
    // EXPECT: input -1 -> FAIL at layer 5 error=E_INPUT
    // EXPECT: input 1500 -> FAIL at layer 5 error=E_PARSE
    // EXPECT: input 7 -> FAIL at layer 5 error=E_VALIDATE
    // EXPECT: input 250 -> FAIL at layer 5 error=E_PROCESS
    // EXPECT: input 0 -> FAIL at layer 5 error=E_VALIDATE
    // EXPECT: input 100 -> value=300 layer=6 status=OK
    // EXPECT: input 3 -> value=106 layer=6 status=OK

    printf("Success: %d Fail: %d\n", success_count, fail_count);    // EXPECT: Success: 3 Fail: 5

    struct Result chain_test = layer_input(10);
    printf("Layer1: val=%d err=%s\n", chain_test.value, err_str(chain_test.error));  // EXPECT: Layer1: val=10 err=OK
    chain_test = layer_parse(10);
    printf("Layer2: val=%d err=%s\n", chain_test.value, err_str(chain_test.error));  // EXPECT: Layer2: val=20 err=OK
    chain_test = layer_validate(10);
    printf("Layer3: val=%d err=%s\n", chain_test.value, err_str(chain_test.error));  // EXPECT: Layer3: val=20 err=OK
    chain_test = layer_process(10);
    printf("Layer4: val=%d err=%s\n", chain_test.value, err_str(chain_test.error));  // EXPECT: Layer4: val=120 err=OK

    printf("Chain test done\n");                                     // EXPECT: Chain test done
    return 0;
}
