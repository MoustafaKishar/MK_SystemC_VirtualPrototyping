#ifndef MEMORY_H
#define MEMORY_H

#include <iostream>
#include <tlm.h>
#include <systemc.h>


// TODO: add your code here
using namespace std;

template<unsigned int memSize = 1024>
class memory : sc_module, tlm::tlm_fw_transport_if<>
{
    private:
        unsigned char mem[memSize];

    public:
        tlm::tlm_target_socket<> tSocket;

        SC_CTOR(memory) : tSocket("tSocket")
        {
            tSocket.bind(*this);
        }

        void b_transport(tlm::tlm_generic_payload &trans, sc_time &delay)
        {
            if (trans.get_address() >= 1024)
            {
                trans.set_response_status( tlm::TLM_ADDRESS_ERROR_RESPONSE );
                return;
            }

            //if (trans.get_data_length() != 1)
            //{
            //    trans.set_response_status( tlm::TLM_BURST_ERROR_RESPONSE );
            //    return;
            //}

            if(trans.get_command() == tlm::TLM_WRITE_COMMAND)
            {
                memcpy(mem+trans.get_address(), // destination                // MK: changed "&mem[trans.get_address()]" to "mem+trans.get_address()"
                    trans.get_data_ptr(),      // source
                    trans.get_data_length());  // size
            }
            else // (trans.get_command() == tlm::TLM_READ_COMMAND)
            {
                memcpy(trans.get_data_ptr(),      // destination
                    mem+trans.get_address(), // source                        // MK: changed "&mem[trans.get_address()]" to "mem+trans.get_address()"
                    trans.get_data_length());  // size
            }

            delay = delay + sc_time(20, SC_NS);                                     // MK: The time consumed by the memory access (20 ns) should be added to the delay variable of the b_transport function call.

            trans.set_response_status( tlm::TLM_OK_RESPONSE );
        }

        // Dummy method
        virtual tlm::tlm_sync_enum nb_transport_fw(
                tlm::tlm_generic_payload& trans,
                tlm::tlm_phase& phase,
                sc_time& delay )
        {
            SC_REPORT_FATAL(this->name(),"nb_transport_fw is not implemented");
            return tlm::TLM_ACCEPTED;
        }

        // Dummy method
        bool get_direct_mem_ptr(tlm::tlm_generic_payload& trans,
                                tlm::tlm_dmi& dmi_data)
        {
            SC_REPORT_FATAL(this->name(),"get_direct_mem_ptr is not implemented");
            return false;
        }

        // Dummy method
        unsigned int transport_dbg(tlm::tlm_generic_payload& trans)
        {
            SC_REPORT_FATAL(this->name(),"transport_dbg is not implemented");
            return 0;
        }

};
#endif // MEMORY_H
