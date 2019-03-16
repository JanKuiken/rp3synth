

double bounds_limit(double value, double min_value, double max_value)
{
    if      (value < min_value) return min_value;
    else if (value > max_value) return max_value;
    else                        return value;
}
