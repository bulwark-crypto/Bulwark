// Copyright (c) 2014 The Bitcoin Core developers
// Copyright (c) 2014-2015 The Dash developers
// Copyright (c) 2015-2017 The PIVX developers
// Copyright (c) 2017-2019 The Bulwark developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "primitives/transaction.h"
#include "main.h"

#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_SUITE(main_tests)

BOOST_AUTO_TEST_CASE(subsidy_limit_test) {
    CAmount nSum = 0;
    for (int nHeight = 0; nHeight < 14000000; nHeight += 1000) {
        /* @TODO fix subsidity, add nBits */
        CAmount nSubsidy = GetBlockValue(nHeight);
        BOOST_CHECK_MESSAGE(nSubsidy <= 50 * COIN,"nSubsidy is " << nSubsidy << ". Expected 50 * COIN.");
        nSum += nSubsidy * 1000;
        BOOST_CHECK_MESSAGE(MoneyRange(nSum),"nSum is " << nSum << ". MaxMoneyOut: " << Params().MaxMoneyOut());
    }
    BOOST_CHECK(nSum == 2099999997690000ULL);
}

BOOST_AUTO_TEST_SUITE_END()
