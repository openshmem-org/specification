main()
{
   static int  race_winner = -1;
   int oldval;
   start_pes(2);
   oldval = shmem_int_cswap(&race_winner, -1, _my_pe(), 0);
   if (oldval == -1) printf("pe %d was first\n",_my_pe());
}
