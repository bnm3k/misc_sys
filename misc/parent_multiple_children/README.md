## Overview

Parent spawns children. Each child sleeps for some time before returning. If by
a given deadline some children are still sleeping, a signal is sent to wake them
up.

## Build

```
make child
make parent
```

## Usage

```
./parent  -n <children> -d <deadline> --min <child-sleep> --max <child-sleep>
```

## Description

- `Parent` forks `n` children. The children then exec `./child`.
- Each child is randomly assigned a time to sleep (in between `min` and `max`).
- Children use `nanosleep` hence the sleep duration can be upto the nanosecond
  resolution.
- After forking the children, parent sets an alarm (via `setitimer`) for the
  deadline. Once this alarm goes off (i.e. the parent receives SIGLARM), parent
  sends SIGUSR1 to any child that has not bee reaped yet.
- On receiving SIGLARM, a child exits with status 1. However, if the child exits
  before the deadline, it does so with status 0.
- Parent also keeps track of the duration in between when a child was forked and
  when they are reaped.
- CLOCK_REALTIME is used for both the alarm and duration tracking.
- It is worth noting that parent uses `sigaction` instead of `signal` to set up
  the handler for SIGALRM since signal defaults to SA_RESTART which will cause
  `wait` to block: we need `wait` to return immediately (with errno EINTR) once
  the alarm goes off so that we can send SIGUSR1 to the children still sleeping.
