/**
 * Copyright (c)      2016 Triad National Security, LLC
 *                         All rights reserved.
 *
 * This file is part of the Gladius project. See the LICENSE.txt file at the
 * top-level directory of this distribution.
 */

/**
 *
 */

#include <iostream>
#include <cstdlib>
#include <string>
#include <cstdio>

#include <limits.h>
#include <unistd.h>
#include <errno.h>

#include "tool-api/gladius-toolbe.h"

#include "mpi.h"

using namespace std;

namespace {

static const int SUCCESS = 0;
static const int ERROR   = 1;

/**
 *
 */
struct Proc {
    char hostname[HOST_NAME_MAX];
    bool leader;
    bool initialized;
    int cwRank;
    int cwSize;
    gladius::toolbe::Tool tool;

    Proc(void)
        : leader(false)
        , initialized(false)
        , cwRank(0)
        , cwSize(0) {
        int rc = gethostname(hostname, sizeof(hostname));
        if (0 != rc) exit(EXIT_FAILURE);
    }

    /**
     *
     */
    ~Proc(void) = default;
};

/**
 *
 */
int
init(
    int argc,
    char **argv,
    Proc &p
) {
    using namespace gladius::toolbe;
    //
    int mpiRC = MPI_Init(&argc, &argv);
    if (MPI_SUCCESS != mpiRC) return ERROR;
    else p.initialized = true;
    //
    mpiRC = MPI_Comm_size(MPI_COMM_WORLD, &p.cwSize);
    if (MPI_SUCCESS != mpiRC) return ERROR;
    //
    mpiRC = MPI_Comm_rank(MPI_COMM_WORLD, &p.cwRank);
    if (MPI_SUCCESS != mpiRC) return ERROR;
    //
    p.leader = (0 == p.cwRank);
    ////////////////////////////////////////////////////////////////////////////
    // Initialize our tool
    ////////////////////////////////////////////////////////////////////////////
    static const bool toolBeVerbose = true;
    if (GLADIUS_SUCCESS != p.tool.create(p.cwRank, toolBeVerbose)) {
        return ERROR;
    }
    // This is a blocking call. Won't return until connection is complete.
    if (GLADIUS_SUCCESS != p.tool.connect()) {
        return ERROR;
    }
    //
    return SUCCESS;
}

/**
 *
 */
int
sayHi(const Proc &p)
{
    if (p.leader) {
        printf("hello from %d processes!\n", p.cwSize);
    }
    return SUCCESS;
}

/**
 *
 */
int
fini(const Proc &p)
{
    if (p.initialized) {
        int mpiRC = MPI_Finalize();
        if (MPI_SUCCESS != mpiRC) return ERROR;
    }
    return SUCCESS;
}

} // namespace

/**
 *
 */
int
main(
    int argc,
    char **argv
) {
    int rc = SUCCESS;
    //
    Proc proc;
    //
    if (SUCCESS != (rc = init(argc, argv, proc))) {
        goto out;
    }
    if (SUCCESS != (rc = sayHi(proc))) {
        goto out;
    }
    // Don't finalize in out: (may hang). Just exit on error so that the MPI
    // runtime can just bail when a process exits with an error code.
    if (SUCCESS != (rc = fini(proc))) {
        goto out;
    }
out:
    int exitCode = (SUCCESS == rc) ? EXIT_SUCCESS : EXIT_FAILURE;
    exit(exitCode);
    // Never reached.
    return -127;
}
