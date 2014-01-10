PROGRAM REMOTEWRITE
INCLUDE 'shmem.fh'

INTEGER BIGD(100)
SAVE BIGD

INTEGER POINTEE(*)
POINTER (PTR,POINTEE)

CALL START_PES(0)


IF (MY_PE() .EQ. 0) THEN
   ! initialize PE 1's BIGD array
   PTR = SHMEM_PTR(BIGD, 1)     ! get address of PE 1's BIGD
                                !   array
   DO I=1,100
        POINTEE(I) = I
   ENDDO
ENDIF

CALL SHMEM_BARRIER_ALL

IF (MY_PE() .EQ. 1) THEN
   PRINT*,'BIGD on PE 1 is: '
   PRINT*,BIGD
ENDIF
END

