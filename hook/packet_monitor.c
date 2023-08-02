#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/netfilter.h>
#include <linux/netfilter_ipv4.h>
#include <linux/ip.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Ouriel, Alexander");
MODULE_DESCRIPTION("task 5");


static unsigned int hook_func(void *priv, struct sk_buff *skb, const struct nf_hook_state *state) {
    struct iphdr *ip_header;
    // extract source and destination IP addresses from the packet
    ip_header = (struct iphdr *)skb_network_header(skb);// get the IP header
    // log the ip addresses of source and destination
    printk(KERN_INFO "Packet Monitor: Received a packet from %pI4 to %pI4\n", &ip_header->saddr, &ip_header->daddr);

    return NF_ACCEPT; // Allow the packet to continue its normal path
}

static struct nf_hook_ops packet_monitor_hook = {
    .hook = hook_func,
    .hooknum = NF_INET_PRE_ROUTING, // Intercept packets just before they are routed (hook type 1)
    .pf = PF_INET, // IPv4 packets
    .priority = NF_IP_PRI_FIRST, // Highest priority
};

static int __init packet_monitor_init(void) {
	nf_register_net_hook(&init_net, &packet_monitor_hook);
  
    printk(KERN_INFO "Packet Monitor module loaded\n");
    return 0;
}

static void __exit packet_monitor_exit(void) {
	nf_unregister_net_hook(&init_net, &packet_monitor_hook);
    printk(KERN_INFO "Packet Monitor module unloaded\n");
}

module_init(packet_monitor_init);
module_exit(packet_monitor_exit);





