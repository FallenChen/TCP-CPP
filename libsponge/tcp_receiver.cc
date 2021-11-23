#include "tcp_receiver.hh"

// Dummy implementation of a TCP receiver

// For Lab 2, please replace with a real implementation that passes the
// automated checks run by `make check_lab2`.

template <typename... Targs>
void DUMMY_CODE(Targs &&... /* unused */) {}

using namespace std;

void TCPReceiver::segment_received(const TCPSegment &seg) {

    // // size_t data_length = 0;
    // static size_t absolute_seqno = 0;
    
    // if(seg.header().syn)
    // {
    //     if(_syn_flag)
    //     {
    //         return;
    //     }
    //     _syn_flag = true;
    //     _isn_set = true;
    //     _isn = seg.header().seqno.raw_value();
    //     _ack_accumulator = 1;
    //     // for checkpoint
    //     absolute_seqno = 1;


    // }
    // // before get a SYN, refuse any segment
    // else if(!_syn_flag)
    // {
    //     return;
    // }
    // else
    // {
    //     absolute_seqno = unwrap(WrappingInt32(seg.header().seqno.raw_value()),WrappingInt32(_isn),absolute_seqno);
    //     // data_length = seg.length_in_sequence_space();
    // }

    // if(seg.header().fin)
    // {
    //     if(_fin_flag)
    //     {
    //         return;
    //     }
    //     _fin_flag = true;
    // }

    // if(seg.length_in_sequence_space() == 0)
    // {
    //     return;
    // }

    // // copy() -> buffer to string
    // // write data to buffer ,index from 0
    // // if fin,means the last segment, not mean contiguous,may be has hole
    // _reassembler.push_substring(seg.payload().copy(), absolute_seqno-1,seg.header().fin);

    // // acutal data length,head_index start from 0
    // _ack_accumulator = _reassembler.head_index() + 1;

    // // means contiguous stream,no hole
    // if(_reassembler.input_ended())
    // {
    //     _ack_accumulator++;
    // }

    
    if(!_isn.has_value())
    {
        // init _isn
        if(seg.header().syn)
        {
            _isn = seg.header().seqno + 1;
        }
        else
        {
            return;
        }
    }

    bool eof = seg.header().fin;

    std::string data = seg.payload().copy();

    size_t index = unwrap(seg.header().seqno + (seg.header().syn ? 1 : 0),
                          _isn.value(), _reassembler.head_index());
    
    _reassembler.push_substring(data, index, eof);    
    
}

optional<WrappingInt32> TCPReceiver::ackno() const 
{ 
    // use _acc_accumulator to get the ackno
    // if(_ack_accumulator > 0)
    // {
    //     return WrappingInt32(wrap(_ack_accumulator, WrappingInt32(_isn)));
    // }
    // else
    // {
    //     return nullopt;
    // }

    if(_isn.has_value())
    {
        return wrap(_reassembler.head_index(), _isn.value()) + (_reassembler.input_ended() ? 1 : 0);
    }
    else
    {
        return nullopt;
    }

}

size_t TCPReceiver::window_size() const 
{
     return _capacity - _reassembler.stream_out().buffer_size();
}
