/*
 * Copyright (c) 2014-2015 Los Alamos National Security, LLC
 *                         All rights reserved.
 *
 * This file is part of the Gladius project. See the LICENSE.txt file at the
 * top-level directory of this distribution.
 */

/**
 * The Front-End (FE) API. The interface to the tool actions.
 */

#ifndef GLADIUS_TOOL_FE_LMON_H_INCLUDED
#define GLADIUS_TOOL_FE_LMON_H_INCLUDED

#include "core/core.h"
#include "core/args.h"
#include "tool-common/tool-common.h"

#include <string>
#include <unistd.h>
#include <sys/types.h>
#include <unistd.h>

#include "lmon_api/lmon_fe.h"
#include "lmon_api/lmon_proctab.h"

namespace gladius {
namespace toolfe {

class LaunchMonFE {
    // Flag indicating whether or not we'll be verbose about our actions.
    bool mBeVerbose = false;
    // The PID of the target application launcher (srun, mpirun, aprun, etc.)
    pid_t mLauncherPID = 0;
    // LMON session number (handle).
    int mSessionNum = 0;
    //
    bool mIsLaunched = false;
    // The hostname of tool front-end.
    std::string mHostname;
    // The name of the tool daemon.
    std::string mToolD;
    // Daemon option string
    std::string mDaemonOpts;
    // LaunchMON install prefix
    std::string mPrefixPath;
    // LaunchMON engine path.
    std::string mEnginePath;
    // What to use for remote login.
    std::string mRemoteLogin;
    // Resource manager info
    lmon_rm_info_t mRMInfo;
    // The process table of the active job.
    toolcommon::ProcessTable mProcTab;
    // The hosts in our job.
    toolcommon::Hosts mHosts;
    //
    int (*mFEToBePackFn)(void *, void *, int, int *) = nullptr;

    ////////////////////////////////////////////////////////////////////////////
    // Private Functions
    ////////////////////////////////////////////////////////////////////////////
    void
    mSetEnvs(void);
    //
    void
    mStartSession(void);
    //
    void
    mEndSession(void);
    //
    void
    mCreateAndPopulateProcTab(void);
    //
    void
    mSetRMInfo(void);

public:
    LaunchMonFE(void);
    //
    ~LaunchMonFE(void);
    //
    void
    init(void);
    //
    void
    launchAndSpawnDaemons(
        const core::Args &appArgs,
        toolcommon::Hosts &outRemoteHosts
    );
    //
    void
    attachAndSpawnDaemons(pid_t launcherPID) {
        GLADIUS_UNUSED(launcherPID);
    }

    /**
     * Shuts down LaunchMON.
     */
    void
    shutdown(void);

    /**
     * Sets whether or not LaunchMON operations will be verbose.
     */
    void
    verbose(bool bVerbose) {
        mBeVerbose = bVerbose;
    }

    /**
     * Returns whether or not daemons have been launched.
     */
    bool
    daemonsLaunched(void) const {
        return mIsLaunched;
    }
    //
    void
    regPackForFeToBe(
        int
        (*packFeBeFn) (
            void *udata,
            void *msgbuf,
            int msgbufmax,
            int *msgBufLen
        )
    );
};

} // end toolfe namespace
} // end gladius namespace

#endif