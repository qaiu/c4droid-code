说明:
MIDI是乐谱文件，因为它只描述如何去发声而不记载实际的音频，所以一般都很小，在几十kb以内，用来做乐器类应用特别适合。

到目前为止只实现了简单的音符提取，其它信息如颤音、力度、换乐器等都未实现，因为都不实用(其实就是懒)。

目前可以正常解析单音轨和同步多音轨，异步多音轨不常见就没做实现了。

对于有损坏的音轨呈半宽容(不会解析失败，但会只解析到出错块就停止解析)，对于格式则是严格的。


用法:
加载MIDI文件(.mid)
	ANS_Mid* mid = ANS_LoadMid(path);
其中path为MIDI文件的路径，加载成功检查无误返回ANS_Mid乐谱结构体指针，失败则返回NULL

这时可以查看mid，但是千万不要修改!

ANS_Mid里有unittick、channel、counts、chunks等成员:
unittick直译为单位时间，用来表示一拍的精度，类似于采样率。
channel表示频道数，多频道用于多乐器演奏，不过在这里将其同化成了一个乐器，是counts数组的长度和chunks的一级长度。
counts记录每一个频道有多少个音符，用下标访问不同频道，比如counts[0]就是查询第一个频道音符数量
chunks记录真正的音符块，一级是频道，二级访问音符块，比如chunks[0][1]就是访问第一个频道的第二个音符块(ANS_MidChunk)

ANS_MidChunk音符块，只读不可修改，专门保存音符等描述信息，成员有tickstamp、action、musicnote:
tickstamp时间戳，从开始到演奏的时间数(单位时间参考ANS_Mid的unittick，比如unittick为480，这个为240则表示为二分之一拍，为960则为两拍的时间)
action动作，有按下(0)和松开(1)，对应的宏是ANS_MIDI_ACTION_PRESSED和ANS_MIDI_ACTION_RELEASED，就是音符的播放和停止。
musicnote音符，60为C4小字一组，数越小音调越低，数越大音调越高，具体参考钢琴琴键。

用ANS_FreeMid(mid)释放分配的结构体

另外的ANS_GetMidStream就是将mid各频道合并在一起，用于方便播放。对应的释放是ANS_DelMidStream。

唯一要注意的一点是传入ANS_GetMidStream的第二个参数要是一个空的指针的地址，整合后可以删除mid，直接用stream加下标的方式读取音符块。