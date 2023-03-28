# A fork() in the road

Overview:

- ways in which fork is a terrible abstraction (for programmers)
- how fork compromises OS implementations
- alternative to fork

## Introduction

- on fork, dirty pages must be downgraded to read-only for copy on write
  mappings

## To read

- The UNIX time-sharing system - Ritchie, Thompson
- The evolution of the Unix time-sharing system - Ritchie
- Fork: http://pubs.opengroup.org/onlinepubs/9699919799/functions/fork.html
- Threads and fork(): think twice before mixing them:
  https://www.linuxprogrammingblog.com/threads-and-fork-think-twice-before-using-them
- Effects of copy-on-write memory management on the response time of UNIX fork
  operations: Jonathan M. Smith
- Chrome fork delay:
  https://bugs.chromium.org/p/chromium/issues/detail?id=819228
- vfork bsd: https://man.freebsd.org/cgi/man.cgi?query=vfork&manpath=2.9.1+BSD
- vfork considered dangerous: https://ewontfix.com/7/
- vfork bug: https://sourceware.org/bugzilla/show_bug.cgi?id=10354
- redis: background saving fails with fork() error under linux:
  https://redis.io/docs/getting-started/faq/
- redis persistence: https://redis.io/docs/management/persistence/
- effect of fork's copy-on-write on fuzzying: "Designing new operating
  primitives to improve fuzzing performance" - Wen Xu
- Sharing and protection in a single-address space operating system - Jeffrey
  Chase.
- Chrome fork bugs: 24-28 (fork in the road)
- printf() anomaly after fork():
  https://stackoverflow.com/questions/2530663/printf-anomaly-after-fork
- POSIX abstractions in modern operating systems: The old, the new and the
  missing - Vaggelis Atlidakis
- node.js spawn() is not asynchronous:
  https://github.com/nodejs/node/issues/14917
- node.js child process tries to reserve as much mem as parent:
  https://github.com/nodejs/node/issues/14917
- posix_spawn as an actual system call:
  https://blogs.oracle.com/solaris/post/posix_spawn-as-an-actual-system-call
- reasoning behind process instead of thread based:
  https://www.postgresql.org/message-id/1098894087.31930.62.camel@localhost.localdomain

## Problems with fork

- not thread safe
- inefficient
- unscalable
- introduces security concerns
- conflates the process and address space: hence hostile to user-mode impl
- does not compose
- user must explicitly flush IO prior to fork, lest output be duplicated
- a forked child inherits everyting from is parent and the programmer is
  responsible for explicitly removing state that the child does not need:
  closing descriptors, isolating namespaces, removing secrets from memory.
- Programs that fork but don't exec render address-space layout randomization
  since each process has the same memory layout: 17
- fork is incompatible with heterogenous hardware: resitricts the definition of
  a process to a single address space

## Advantages of the Fork API

- Fork is simple
- space between fork and exec
- Creating a process with fork is orthogonal to starting a new program.
- fork eased concurrency

## Replacing fork

- high-level spawn API and a lower-level microkernel-like API
- Alternatives to fork without exec
