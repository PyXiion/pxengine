void main()
{
  Matrix4x4 mat = {1,   2,  3,  4,
                   5,   6,  7,  8,
                   9,   10, 11, 12,
                   13,  14, 15, 16};

  for (int j = 0; j < 4; j++)
  {
    for (int k = 0; k < 4; k++)
    {
      print(mat.get(j,k) + ",");
    }
    println("");
  }
}
