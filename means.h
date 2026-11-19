#ifndef MEANS_H
#define MEANS_H

/* language-wide constants and conversion policy */

#define MEANS_NOT_FOUND   (-1)
#define MEANS_FIRST_POS    (1)

#define TO_MEANS_INDEX(c_idx) \
    ((c_idx) < 0 ? MEANS_NOT_FOUND : (c_idx) + MEANS_FIRST_POS)

#define FROM_MEANS_INDEX(means_idx) \
    ((means_idx) <= 0 ? -1 : (means_idx) - MEANS_FIRST_POS)

/* future home for other language-level knobs
   (max params, default precision, …) */

#endif /* MEANS_H */