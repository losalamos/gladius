/*
 * Copyright (c) 2014-2015 Los Alamos National Security, LLC
 *                         All rights reserved.
 *
 * Copyright (c) 2008-2012, Lawrence Livermore National Security, LLC
 *
 * This file is part of the Gladius project. See the LICENSE.txt file at the
 * top-level directory of this distribution.
 */

/**
 * Implements LaunchMON actions shim.
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "lmon/lmon-be.h"

#include "core/core.h"
#include "tool-common/tool-common.h"

#include <iomanip>

using namespace gladius;
using namespace gladius::lmonbe;

namespace {
// This component's name.
const std::string CNAME = "lmon-be";
// CNAME's color code.
const std::string NAMEC =
    core::colors::color().ansiBeginColor(core::colors::NONE);
// Convenience macro to decorate this component's output.
#define COMP_COUT GLADIUS_COMP_COUT(CNAME, NAMEC)
// Output if this component is being verbose.
#define VCOMP_COUT(streamInsertions)                                           \
do {                                                                           \
    if (this->mBeVerbose) {                                                    \
        COMP_COUT << streamInsertions;                                         \
    }                                                                          \
} while (0)
}


////////////////////////////////////////////////////////////////////////////////
// LaunchMonBE
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
namespace LaunchMonBEGlobals {
}

namespace {
int
unpackToolFEInfo(
    void *buf,
    int bufLen,
    void *data
) {
    return 0;
}
}

/**
 *
 */
LaunchMonBE::LaunchMonBE(
    void
) : mBeVerbose(false)
{
}

/**
 *
 */
LaunchMonBE::~LaunchMonBE(void)
{
}

/**
 *
 */
void
LaunchMonBE::init(
    const core::Args &args,
    bool beVerbose
)
{
    mBeVerbose = beVerbose;
    mArgs = args;
    //
    auto argc = mArgs.argc();
    auto **argv = mArgs.argv();
    auto status = LMON_be_init(LMON_VERSION, &argc, &argv);
    if (LMON_OK != status) {
        GLADIUS_THROW_CALL_FAILED_RC("LMON_be_init", status);
    }
    status = LMON_be_regUnpackForFeToBe(unpackToolFEInfo);
    if (LMON_OK != status) {
        GLADIUS_THROW_CALL_FAILED_RC("LMON_be_regUnpackForFeToBe", status);
    }
    status = LMON_be_handshake(NULL);
    if (LMON_OK != status) {
        GLADIUS_THROW_CALL_FAILED_RC("LMON_be_handshake", status);
    }
    status = LMON_be_ready(NULL);
    if (LMON_OK != status) {
        GLADIUS_THROW_CALL_FAILED_RC("LMON_be_ready", status);
    }
    //
    mCreateAndPopulateProcTab();
}

/**
 *
 */
void
LaunchMonBE::mCreateAndPopulateProcTab(void)
{
    try {
        int numProcTabEntries = 0;
        auto rc = LMON_be_getMyProctabSize(&numProcTabEntries);
        if (LMON_OK != rc) {
            GLADIUS_THROW_CALL_FAILED_RC("LMON_be_getMyProctabSize", rc);
        }
        // Allocate room for the entries.
        mProcTab = toolcommon::ProcessTable(numProcTabEntries);
        // Now populate the thing...
        int pSize = 0;
        rc = LMON_be_getMyProctab(
                 mProcTab.procTab(),
                 &pSize,
                 numProcTabEntries // Max Length
             );
        if (LMON_OK != rc) {
            GLADIUS_THROW_CALL_FAILED_RC("LMON_fe_getProctable", rc);
        }
        if (true) {
            std::cout << "Done Getting Process Table" << std::endl;
            mProcTab.dumpTo(std::cout);
        }
    }
    catch (const std::exception &e) {
        throw core::GladiusException(GLADIUS_WHERE, e.what());
    }
}
