int printf(const char *fmt, ...);

struct Inner {
    int val;
    int extra;
};

struct Middle {
    struct Inner inner;
    int mid_val;
};

struct Outer {
    struct Middle middle;
    int out_val;
};

int sum_outer(struct Outer *o) {
    return o->out_val + o->middle.mid_val + o->middle.inner.val + o->middle.inner.extra;
}

void init_outer(struct Outer *o, int a, int b, int c, int d) {
    o->out_val = a;
    o->middle.mid_val = b;
    o->middle.inner.val = c;
    o->middle.inner.extra = d;
}

void scale_inner(struct Inner *i, int factor) {
    i->val = i->val * factor;
    i->extra = i->extra * factor;
}

void add_to_middle(struct Middle *m, int delta) {
    m->mid_val = m->mid_val + delta;
    m->inner.val = m->inner.val + delta;
    m->inner.extra = m->inner.extra + delta;
}

int nested_product(struct Outer *o) {
    return o->out_val * o->middle.mid_val * o->middle.inner.val;
}

void swap_inner_fields(struct Inner *i) {
    int tmp = i->val;
    i->val = i->extra;
    i->extra = tmp;
}

void copy_outer(struct Outer *dst, struct Outer *src) {
    dst->out_val = src->out_val;
    dst->middle.mid_val = src->middle.mid_val;
    dst->middle.inner.val = src->middle.inner.val;
    dst->middle.inner.extra = src->middle.inner.extra;
}

int main(void) {
    struct Outer o1;
    init_outer(&o1, 10, 20, 30, 40);

    // EXPECT: outer=10 mid=20 inner_val=30 inner_extra=40
    printf("outer=%d mid=%d inner_val=%d inner_extra=%d\n",
           o1.out_val, o1.middle.mid_val, o1.middle.inner.val, o1.middle.inner.extra);

    // EXPECT: sum=100
    printf("sum=%d\n", sum_outer(&o1));

    scale_inner(&o1.middle.inner, 3);
    // EXPECT: after scale: val=90 extra=120
    printf("after scale: val=%d extra=%d\n", o1.middle.inner.val, o1.middle.inner.extra);

    add_to_middle(&o1.middle, 5);
    // EXPECT: after add: mid=25 val=95 extra=125
    printf("after add: mid=%d val=%d extra=%d\n",
           o1.middle.mid_val, o1.middle.inner.val, o1.middle.inner.extra);

    // EXPECT: product=23750
    printf("product=%d\n", nested_product(&o1));

    swap_inner_fields(&o1.middle.inner);
    // EXPECT: swapped: val=125 extra=95
    printf("swapped: val=%d extra=%d\n", o1.middle.inner.val, o1.middle.inner.extra);

    struct Outer o2;
    copy_outer(&o2, &o1);
    // EXPECT: copy: outer=10 mid=25 val=125 extra=95
    printf("copy: outer=%d mid=%d val=%d extra=%d\n",
           o2.out_val, o2.middle.mid_val, o2.middle.inner.val, o2.middle.inner.extra);

    // EXPECT: equal=1
    printf("equal=%d\n", sum_outer(&o1) == sum_outer(&o2));

    o2.middle.inner.val = 0;
    o2.middle.inner.extra = 0;
    // EXPECT: zeroed sum=35
    printf("zeroed sum=%d\n", sum_outer(&o2));

    struct Outer arr[3];
    int i;
    for (i = 0; i < 3; i = i + 1) {
        init_outer(&arr[i], i * 10, i * 20, i * 30, i * 40);
    }

    // EXPECT: arr[0] sum=0
    printf("arr[0] sum=%d\n", sum_outer(&arr[0]));
    // EXPECT: arr[1] sum=100
    printf("arr[1] sum=%d\n", sum_outer(&arr[1]));
    // EXPECT: arr[2] sum=200
    printf("arr[2] sum=%d\n", sum_outer(&arr[2]));

    int total = 0;
    for (i = 0; i < 3; i = i + 1) {
        total = total + arr[i].middle.inner.val;
    }
    // EXPECT: total inner vals=90
    printf("total inner vals=%d\n", total);

    arr[1].middle.inner.val = 999;
    // EXPECT: modified arr[1] inner=999
    printf("modified arr[1] inner=%d\n", arr[1].middle.inner.val);

    // EXPECT: arr[1] product=199800
    printf("arr[1] product=%d\n", nested_product(&arr[1]));

    return 0;
}
