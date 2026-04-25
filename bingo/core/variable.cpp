#pragma once

#include "core.cpp"

namespace vars {
    struct __attribute__((packed)) variable : bingo::Number {
        union {
            struct {
                char letter;
                uint64_t number : 7*8 = 0;
            } __attribute__((packed));
            int64_t raw;
        };

        explicit variable(const char letter, const uint64_t number = 0) : letter(letter), number(number) {
            this->pattern = bingo::var(raw).pattern;
        }

        variable operator()(const uint64_t num = 0) const {
            variable v = *this;
            v.number = num;
            return v;
        }

        Number operator()(bingo::detail::Result res) const {
            return res[raw];
        }

        Number operator()(bingo::detail::OResult res) const {
            return res.value()[raw];
        }

        operator int64_t() const {
            return raw;
        }
    };

#define VAR(n) variable n{#n[0]}
    VAR(a); VAR(b); VAR(c); VAR(d); VAR(e); VAR(f); VAR(g); VAR(h); VAR(i); VAR(j); VAR(k); VAR(l); VAR(m);
    VAR(n); VAR(o); VAR(p); VAR(q); VAR(r); VAR(s); VAR(t); VAR(u); VAR(v); VAR(w); VAR(x); VAR(y); VAR(z);

    VAR(A); VAR(B); VAR(C); VAR(D); VAR(E); VAR(F); VAR(G); VAR(H); VAR(I); VAR(J); VAR(K); VAR(L); VAR(M);
    VAR(N); VAR(O); VAR(P); VAR(Q); VAR(R); VAR(S); VAR(T); VAR(U); VAR(V); VAR(W); VAR(X); VAR(Y); VAR(Z);

#define CONTEXTVAR(n) bingo::Number n(const uint64_t num = 0) const { return ctx[variable{#n[0], num}.raw];}
    struct context {
        bingo::detail::Result&& ctx;

        CONTEXTVAR(a); CONTEXTVAR(b); CONTEXTVAR(c); CONTEXTVAR(d); CONTEXTVAR(e); CONTEXTVAR(f);
        CONTEXTVAR(g); CONTEXTVAR(h); CONTEXTVAR(i); CONTEXTVAR(j); CONTEXTVAR(k); CONTEXTVAR(l);
        CONTEXTVAR(m); CONTEXTVAR(n); CONTEXTVAR(o); CONTEXTVAR(p); CONTEXTVAR(q); CONTEXTVAR(r);
        CONTEXTVAR(s); CONTEXTVAR(t); CONTEXTVAR(u); CONTEXTVAR(v); CONTEXTVAR(w); CONTEXTVAR(x);
        CONTEXTVAR(y); CONTEXTVAR(z);

        CONTEXTVAR(A); CONTEXTVAR(B); CONTEXTVAR(C); CONTEXTVAR(D); CONTEXTVAR(E); CONTEXTVAR(F);
        CONTEXTVAR(G); CONTEXTVAR(H); CONTEXTVAR(I); CONTEXTVAR(J); CONTEXTVAR(K); CONTEXTVAR(L);
        CONTEXTVAR(M); CONTEXTVAR(N); CONTEXTVAR(O); CONTEXTVAR(P); CONTEXTVAR(Q); CONTEXTVAR(R);
        CONTEXTVAR(S); CONTEXTVAR(T); CONTEXTVAR(U); CONTEXTVAR(V); CONTEXTVAR(W); CONTEXTVAR(X);
        CONTEXTVAR(Y); CONTEXTVAR(Z);
    };
}
