; Raw spinlock implementation that is used by both the kernel space and the user space part of our API.
; A spinlock is represented by a single integer.
; If its value is
;    0, the spinlock is acquired
;    1, the spinlock is not acquired
;   <0, the spinlock is acquired and a third thread tried to acquire it too


pause_once:
  push rbp
  mov rbp, rsp
  pause
  pop rbp
  ret


; Acquires the given raw spinlock; if it is already acquired by another thread, this function blocks.
; Parameters:
;     - Pointer to spinlock value.
[global raw_spinlock_lock]
raw_spinlock_lock:
	
	; Try to decrement spinlock value (atomic)
	lock dec byte [rdi]
	
	; Value >= 0? -> Acquiring was successful
	jns raw_spinlock_lock_success
	
	; Acquiring failed, reset value
	lock inc byte [rdi]
	
	; This thread is blocked, wait
	call pause_once
	
	; Try again
	jmp raw_spinlock_lock

raw_spinlock_lock_success:

	; Acquiring successful
	ret


; Releases the given raw spinlock.
; Parameters:
;     - Pointer to spinlock value.
[global raw_spinlock_unlock]
raw_spinlock_unlock:
	
	; Release spinlock by incrementing its value
	lock inc byte [rdi]
	
	; Done
	ret
