Matrix4i m = Matrix4i::Random();
cout << "Here is the matrix m:" << endl
     << m << endl;
cout << "Here is m.topRightCorner<2,Dynamic>(2,2):" << endl;
cout << m.topRightCorner<2, Dynamic>(2, 2) << endl;
m.topRightCorner<2, Dynamic>(2, 2).setZero();
cout << "Now the matrix m is:" << endl
     << m << endl;
