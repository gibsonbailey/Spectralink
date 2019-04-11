int* find_max(const cv::Mat& src[], int * dst, int x_partitions, int y_partitions)
{
    int max = 0;
    int x_interval = ceiling(src.cols / x_partitions);
    int y_interval = ceiling(src.rows / y_partitions);
 
    int dst_counter = 0;

    for (int m = 0; m < x_partitions; m++)
    {
        for (int n = 0; n < y_partitions; n++)
        {
            max = 0;
            for (int i = m*x_interval; i <(m+1)*x_interval; i++)
            {
                                                          
                for (int j = n*y_interval; j < (n+1)*y_interval; j++)
                {
                    if (dst[i][j] > max)
                    {
                        max = src[i][j];
                    }
                }
            }
            dst[dst_counter] = max;
            dst_counter++;
        }
    }
    return dst;
}
