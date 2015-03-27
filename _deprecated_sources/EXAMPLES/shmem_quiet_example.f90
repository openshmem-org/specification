PROGRAM COMPFLAG
INTEGER FLAG_VAR, ARRAY(100), RECEIVER, SENDER
COMMON/FLAG/FLAG_VAR
COMMON/DATA/ARRAY
INTRINSIC SHMEM_MY_PE()

FLAG_VAR = 0
CALL SHMEM_BARRIER_ALL          ! wait for FLAG_VAR to be initialized

SENDER = 0                      ! PE 0 sends the data
RECEIVER = 1                    ! PE 1 receives the data
IF (SHMEM_MY_PE() .EQ. 0) THEN
   ARRAY = 33
   CALL SHMEM_PUT(ARRAY, ARRAY, 100, RECEIVER)   ! start sending data
   CALL SHMEM_QUIET                              ! wait for delivery
   CALL SHMEM_PUT(FLAG_VAR, 1, 1, RECEIVER)      ! send completion flag
ELSE IF (SHMEM_MY_PE() .EQ. RECEIVER) THEN
   CALL SHMEM_WAIT(FLAG_VAR, 0)
   PRINT*,ARRAY                  ! ARRAY has been delivered
ENDIF
END
