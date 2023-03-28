# Parent dies before child

- When a parent dies before the child, the kernel makes the child into 'orphans'
  and assigns `init` as their parent.
- init process checks periodically for new children and waits for them.
- If a child daemonized itself, then it's no longer the child of the parent that
  forked it.

## References

- https://stackoverflow.com/a/395883
