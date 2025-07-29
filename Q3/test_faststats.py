import faststats

data = [1.0, 2.0, 3.0, 2.0, 4.0, 5.0, 2.0]

print("Data:", data)
print("Sum:", faststats.sum_values(data))
print("Mean:", faststats.mean_values(data))
print("Standard Deviation:", faststats.std_dev(data))
print("Mode:", faststats.mode_value(data))
print("Count:", faststats.count_values(data))
