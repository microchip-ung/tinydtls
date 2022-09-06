/*******************************************************************************
 *
 * Copyright (c) 2022 Microchip Technologies and others.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * and Eclipse Distribution License v. 1.0 which accompanies this distribution.
 *
 * The Eclipse Public License is available at http://www.eclipse.org/legal/epl-v10.html
 * and the Eclipse Distribution License is available at
 * http://www.eclipse.org/org/documents/edl-v10.php.
 *
 *******************************************************************************/

#include "tinydtls.h"
#include "dtls_prng.h"
#include "lm_tinydtls.h"

int dtls_prng(unsigned char *buf, size_t len)
{
    return lm_tinydtls_prng(buf, len);
}

void dtls_prng_init(unsigned seed)
{
    lm_tinydtls_prng_init(seed);
}
